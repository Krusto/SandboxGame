#include "ChunkMesh.hpp"
#include "BlockVertexData.hpp"

#include <algorithm>
#include <bitset>
#include <future>
#include <unordered_map>
#include <Core/Timer.hpp>

#include <Renderer/Renderer.hpp>

namespace Engine
{
    void ChunkMesh::Generate(const BlockData* blockData)
    {
        GenerateVertexData(blockData, m_Mesh.blocks, m_Mesh.vertices, m_Mesh.indices);
    }

    RendererCommand ChunkMesh::GetGenerateCommand(ChunkMesh* mesh)
    {
        return RendererCommand([&]() { ChunkMesh::UploadData(mesh); });
    }

    void ChunkMesh::UploadData(ChunkMesh* mesh)
    {
        if (mesh->m_Mesh.vertices.size() != 0)
        {
            mesh->m_VertexArray = Engine::VertexArray::Create(mesh->m_Mesh.indices.size());
            mesh->m_VertexArray.Bind();
            // mesh->m_VertexArray.AddVertexBuffer(VoxelVertex::GetLayout(), (float*) mesh->m_Mesh.vertices.data(),
            //                                      mesh->m_Mesh.vertices.size());

            mesh->m_VertexArray.AddIndexBuffer(mesh->m_Mesh.indices.data(), mesh->m_Mesh.indices.size());

            mesh->m_BlocksBuffer = Engine::StorageBuffer::Create(mesh->m_Mesh.blocks.data(), mesh->m_Mesh.blocks.size(),
                                                                 StorageBufferType::MapCoherent);
            mesh->m_QuadsBuffer = Engine::StorageBuffer::Create((uint8_t*) mesh->m_Mesh.vertices.data(),
                                                                mesh->m_Mesh.vertices.size() * sizeof(VoxelVertex),
                                                                StorageBufferType::MapCoherent);
        }
    }

    void ChunkMesh::GenerateFaceLayer(const BlockData* blockData, uint32_t axis, uint32_t* data)
    {
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            for (size_t z = 0; z < CHUNK_SIZE; z++)
            {
                for (size_t x = 0; x < CHUNK_SIZE; x++)
                {
                    if (blockData->IsSolid(glm::ivec3(x, y, z)))
                    {
                        (data)[z * CHUNK_SIZE + x] |= 1 << (uint32_t) y;
                        (data)[y * CHUNK_SIZE + z + CHUNK_SIZE_SQUARE] |= 1 << (uint32_t) x;
                        (data)[y * CHUNK_SIZE + x + (size_t) 2 * CHUNK_SIZE_SQUARE] |= 1 << (uint32_t) z;
                    }
                }
            }
        }
    }

    void ChunkMesh::GenerateVertexData(const BlockData* blockData, std::vector<uint8_t>& blocks,
                                       std::vector<VoxelVertex>& vertices, std::vector<uint32_t>& indices)
    {


        uint32_t* face_layers = Engine::Allocator::AllocateArray<uint32_t>(CHUNK_SIZE_SQUARE * 3);
        std::memset(face_layers, 0, (size_t) CHUNK_SIZE_SQUARE * 3 * 4);
        GenerateFaceLayer(blockData, 0, face_layers);

        std::vector<uint32_t> col_face_map[6];
        for (size_t axis = 0; axis < 3; axis++)
        {
            col_face_map[axis * 2].resize(CHUNK_SIZE_SQUARE);
            col_face_map[axis * 2 + 1].resize(CHUNK_SIZE_SQUARE);
            for (size_t index = 0; index < CHUNK_SIZE_SQUARE; index++)
            {
                uint32_t col = face_layers[CHUNK_SIZE_SQUARE * axis + index];

                col_face_map[axis * 2 + 0][index] = col & ~(col << 1);
                col_face_map[axis * 2 + 1][index] = col & ~(col >> 1);
            }
        }

        std::unordered_map<uint8_t, std::unordered_map<uint32_t, std::vector<uint32_t>>> data[6];
        for (size_t axis = 0; axis < 6; axis++)
        {
            for (size_t z = 0; z < CHUNK_SIZE; z++)
            {
                for (size_t x = 0; x < CHUNK_SIZE; x++)
                {
                    uint32_t col_index = x + (z * CHUNK_SIZE);
                    uint32_t col = col_face_map[axis][col_index];
                    while (col != 0)
                    {
                        uint32_t y = std::countr_zero(col);
                        uint32_t mask = ~(1 << y);
                        col = col & mask;

                        glm::vec3 voxelPos(x, z, y);
                        switch (axis)
                        {
                            case 0:
                            case 1:
                                voxelPos = glm::vec3(x, y, z);
                                break;
                            case 2:
                            case 3:
                                voxelPos = glm::vec3(y, z, x);
                                break;
                        }

                        uint8_t voxel = blockData->GetBlock(voxelPos);
                        auto& buff = data[axis][voxel][y];
                        if (data[axis][voxel][y].size() < CHUNK_SIZE) data[axis][voxel][y].resize(CHUNK_SIZE);
                        buff[x] |= 1 << (uint32_t) z;
                    }
                }
            }
        }

        for (size_t axis = 0; axis < 6; axis++)
        {
            auto& planesAtCurrentAxis = data[axis];
            for (auto& [block, planes]: planesAtCurrentAxis)
            {
                for (auto& [y, plane]: planes)
                {
                    auto quads = BinaryGreedyMesherPlane(&plane[0], axis);

                    for (auto& quad: quads)
                    {
                        InsertFace(vertices, axis, quad, y);
                        blocks.push_back(block);
                    }
                }
            }
        }

        GenerateIndices(indices, vertices.size());

        Engine::Allocator::Deallocate(face_layers);
    }

    std::vector<Quad> ChunkMesh::BinaryGreedyMesherPlane(uint32_t* faceData, uint32_t axis)
    {
        std::vector<Quad> quads;
        quads.reserve(10);
        for (size_t x = 0; x < CHUNK_SIZE; x++)
        {
            uint32_t y = 0;
            while (y < CHUNK_SIZE)
            {
                y += std::countr_zero(faceData[x] >> y);
                if (y >= CHUNK_SIZE) continue;

                uint32_t h = std::countr_one(faceData[x] >> y);
                uint32_t hmask = (uint32_t) (((uint64_t) 1 << h) - 1);
                uint32_t mask = hmask << y;
                uint32_t w = 1;
                while (x + w < CHUNK_SIZE)
                {
                    uint32_t next_h = (faceData[x + w] >> y) & hmask;
                    if (next_h != hmask) { break; }

                    faceData[x + w] = faceData[x + w] & (~mask);
                    w++;
                }
                quads.push_back({.x = x, .y = y, .w = w, .h = h});
                y += h;
            }
        }
        return quads;
    }

    void ChunkMesh::InsertFace(std::vector<VoxelVertex>& vertices, uint32_t axis, const Quad& quad, uint32_t z)
    {

        for (uint32_t i = 0; i < 1; i++)
        {
            VoxelVertex vertex{};

            vertex.compressedData = (uint32_t) quad.x & 0x1Fu;
            vertex.compressedData |= ((uint32_t) quad.y & 0x1Fu) << 5;
            vertex.compressedData |= ((uint32_t) z & 0x1Fu) << 10;
            vertex.compressedData |= ((uint32_t) quad.w - 1) << 15;
            vertex.compressedData |= ((uint32_t) quad.h - 1) << 20;
            vertex.compressedData |= ((uint32_t) axis) << 25;

            vertices.push_back(vertex);
        }
    }

    void ChunkMesh::GenerateIndices(std::vector<uint32_t>& indices, uint32_t numQuads)
    {
        indices.reserve(numQuads * 6);
        for (uint32_t i = 0; i < numQuads; i++)
        {
            indices.push_back(i * 4 + 0);
            indices.push_back(i * 4 + 1);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 0);
            indices.push_back(i * 4 + 2);
            indices.push_back(i * 4 + 3);
        }
    }

    std::array<VoxelVertex, 4> ChunkMesh::GetVertices(uint32_t axis, const Quad& quad, uint32_t z)
    {
        std::array<VoxelVertex, 4> vertices{};
        for (uint32_t i = 0; i < 4; i++)
        {
            vertices[i].compressedData |= axis;
            vertices[i].compressedData |= ((uint32_t) quad.w & 0xFF) << 8;
            vertices[i].compressedData |= ((uint32_t) quad.h & 0xFF) << 16;
        }
        return vertices;
    }

    void ChunkMesh::Destroy()
    {
        if (m_VertexArray.IsValid())
        {
            m_VertexArray.Destroy();
            m_BlocksBuffer.Destroy();
            m_QuadsBuffer.Destroy();
        }
    }
}// namespace Engine
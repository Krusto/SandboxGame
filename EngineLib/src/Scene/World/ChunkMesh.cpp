#include "ChunkMesh.hpp"
#include "BlockVertexData.hpp"
#include "ChunkRenderer.hpp"

#include <algorithm>
#include <bitset>
#include <bit>
#include <future>
#include <unordered_map>
#include <Core/Timer.hpp>

#include <Renderer/Renderer.hpp>

namespace Engine
{

    static inline const int getAxisIndex(const int axis, const int a, const int b, const int c)
    {
        if (axis == 0) return b + (a * CHUNK_SIZE_PADDED) + (c * CHUNK_SIZE_SQUARE_PADDED);
        else if (axis == 1)
            return b + (c * CHUNK_SIZE_PADDED) + (a * CHUNK_SIZE_SQUARE_PADDED);
        else
            return c + (a * CHUNK_SIZE_PADDED) + (b * CHUNK_SIZE_SQUARE_PADDED);
    }

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

    void ChunkMesh::GenerateVertexData2(const BlockData* blockData, std::vector<uint8_t>& blocks,
                                        std::vector<VoxelVertex>& vertices, std::vector<uint32_t>& indices)
    {
        constexpr uint64_t P_MASK = ~(1ull << 63 | 1);

        m_Mesh.opaqueMask = Engine::Allocator::AllocateArray<uint64_t>(CHUNK_SIZE_SQUARE_PADDED);
        m_Mesh.faceMasks = Engine::Allocator::AllocateArray<uint64_t>(CHUNK_SIZE_SQUARE * 6);
        m_Mesh.forwardMerged = Engine::Allocator::AllocateArray<uint8_t>(CHUNK_SIZE_SQUARE);
        m_Mesh.rightMerged = Engine::Allocator::AllocateArray<uint8_t>(CHUNK_SIZE);

        memset(m_Mesh.opaqueMask, 0, CHUNK_SIZE_SQUARE_PADDED * sizeof(uint64_t));
        memset(m_Mesh.faceMasks, 0, (CHUNK_SIZE_SQUARE + 6) * sizeof(uint64_t));
        memset(m_Mesh.forwardMerged, 0, CHUNK_SIZE_SQUARE * sizeof(uint8_t));
        memset(m_Mesh.rightMerged, 0, CHUNK_SIZE * sizeof(uint8_t));

        uint64_t* opaqueMask = m_Mesh.opaqueMask;
        uint64_t* faceMasks = m_Mesh.faceMasks;
        uint8_t* forwardMerged = m_Mesh.forwardMerged;
        uint8_t* rightMerged = m_Mesh.rightMerged;

        for (int a = 1; a < CHUNK_SIZE_PADDED - 1; a++)
        {
            const int aCHUNK_SIZE_PADDED = a * CHUNK_SIZE_PADDED;
            for (int b = 1; b < CHUNK_SIZE_PADDED; b++)
            {
                const uint64_t columnBits = opaqueMask[(a * CHUNK_SIZE_PADDED) + b] & P_MASK;
                const int baIndex = (b - 1) + (a - 1) * CHUNK_SIZE;
                const int abIndex = (a - 1) + (b - 1) * CHUNK_SIZE;

                faceMasks[baIndex + 0 * CHUNK_SIZE_SQUARE] =
                        (columnBits & ~opaqueMask[aCHUNK_SIZE_PADDED + CHUNK_SIZE_PADDED + b]) >> 1;
                faceMasks[baIndex + 1 * CHUNK_SIZE_SQUARE] =
                        (columnBits & ~opaqueMask[aCHUNK_SIZE_PADDED - CHUNK_SIZE_PADDED + b]) >> 1;

                faceMasks[abIndex + 2 * CHUNK_SIZE_SQUARE] =
                        (columnBits & ~opaqueMask[aCHUNK_SIZE_PADDED + (b + 1)]) >> 1;
                faceMasks[abIndex + 3 * CHUNK_SIZE_SQUARE] =
                        (columnBits & ~opaqueMask[aCHUNK_SIZE_PADDED + (b - 1)]) >> 1;

                faceMasks[baIndex + 4 * CHUNK_SIZE_SQUARE] = (columnBits & ~(opaqueMask[aCHUNK_SIZE_PADDED + b] >> 1));
                faceMasks[baIndex + 5 * CHUNK_SIZE_SQUARE] = (columnBits & ~(opaqueMask[aCHUNK_SIZE_PADDED + b] << 1));
            }
        }

        int vertexI = {};
        //Greedy messhing faces 0-3
        for (int face = 0; face < 4; face++)
        {
            const int axis = face / 2;
            const int faceVertexBegin = vertexI;
            for (int layer = 0; layer < CHUNK_SIZE; layer++)
            {
                const int bitsLocation = layer * CHUNK_SIZE + face * CHUNK_SIZE_SQUARE;

                for (int forward = 0; forward < CHUNK_SIZE; forward++)
                {
                    uint64_t bitsHere = faceMasks[forward + bitsLocation];
                    if (bitsHere == 0) continue;

                    const uint64_t bitsNext = forward + 1 < CHUNK_SIZE ? faceMasks[(forward + 1) + bitsLocation] : 0;

                    uint8_t rightMerged = 1;
                    while (bitsHere)
                    {
                        unsigned long bitPos = std::countr_zero(bitsHere);
                        const uint8_t type =
                                blockData->RawData()[getAxisIndex(axis, forward + 1, bitPos + 1, layer + 1)];
                        uint8_t& forwardMergedRef = forwardMerged[bitPos];
                        if ((bitsNext >> bitPos & 1) &&
                            type == blockData->RawData()[getAxisIndex(axis, forward + 2, bitPos + 1, layer + 1)])
                        {
                            forwardMergedRef++;
                            bitsHere &= ~(1ull << bitPos);
                            continue;
                        }
                        for (int right = bitPos + 1; right < CHUNK_SIZE; right++)
                        {
                            if (!(bitsHere >> right & 1) || forwardMergedRef != forwardMerged[right] ||
                                type != blockData->RawData()[getAxisIndex(axis, forward + 1, right + 1, layer + 1)])
                                break;
                            forwardMerged[right] = 0;
                            rightMerged++;
                        }
                        bitsHere &= ~((1ull << (bitPos + rightMerged)) - 1);

                        const uint8_t meshFront = forward - forwardMergedRef;
                        const uint8_t meshLeft = bitPos;
                        const uint8_t meshUp = layer + (~face & 1);

                        const uint8_t meshWidth = rightMerged;
                        const uint8_t meshLength = forwardMergedRef + 1;

                        forwardMergedRef = 0;
                        rightMerged = 1;

                        uint64_t quad;
                        switch (face)
                        {
                            case 0:
                            case 1:
                                InsertFace(vertices, axis,
                                           Quad{(uint64_t) meshFront + (face == 1 ? meshLength : 0), meshUp, meshLeft,
                                                meshLength},
                                           meshWidth);
                                vertexI++;
                                blocks.push_back(type);
                                break;
                            case 2:
                            case 3:
                                InsertFace(vertices, axis,
                                           Quad{(uint64_t) meshUp, (uint64_t) meshFront + (face == 2 ? meshLength : 0),
                                                meshLeft, meshLength},
                                           meshWidth);
                                vertexI++;
                                blocks.push_back(type);
                                break;
                        }
                    }
                }
            }
        }

        GenerateIndices(indices, vertices.size());

        Engine::Allocator::Deallocate(m_Mesh.opaqueMask);
        Engine::Allocator::Deallocate(m_Mesh.faceMasks);
        Engine::Allocator::Deallocate(m_Mesh.forwardMerged);
        Engine::Allocator::Deallocate(m_Mesh.rightMerged);
    }

    void ChunkMesh::GenerateVertexData(const BlockData* blockData, std::vector<uint8_t>& blocks,
                                       std::vector<VoxelVertex>& vertices, std::vector<uint32_t>& indices)
    {
        {
            ScopedTimer timer("Vertex Data Generation");
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
            Engine::Allocator::Deallocate(face_layers);
        }
        GenerateIndices(indices, vertices.size());
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
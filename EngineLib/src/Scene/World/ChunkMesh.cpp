#include "ChunkMesh.hpp"
#include "BlockVertexData.hpp"

#include <algorithm>
#include <bitset>
#include <future>
#include <unordered_map>
#include <Core/Timer.hpp>

namespace Engine
{
    void ChunkMesh::Generate(const BlockData* blockData)
    {
        ScopedTimer timer("ChunkMesh::Generate");
        std::vector<VoxelVertex> vertices;
        std::vector<uint32_t> indices;
        uint32_t maxIndex = 0;

        GenerateVertexData(blockData, vertices, indices, maxIndex);

        m_VertexArray = Engine::VertexArray::Create(indices.size());
        m_VertexArray->Bind();
        m_VertexArray->AddVertexBuffer(
                Engine::VertexBuffer::Create(VoxelVertex::GetLayout(), (float*) vertices.data(), vertices.size()));
        m_VertexArray->AddIndexBuffer(Engine::IndexBuffer::Create(indices.data(), indices.size()));
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

    void ChunkMesh::GenerateVertexData(const BlockData* blockData, std::vector<VoxelVertex>& vertices,
                                       std::vector<uint32_t>& indices, uint32_t& maxIndex)
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

        std::vector<std::thread> threads;

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
                        uint32_t newY = y;
                        std::array<VoxelVertex, 4> vertices_to_add = GetVertices(axis, quad, newY);
                        vertices_to_add[0].texCoord = {1.0f, 1.0f};
                        vertices_to_add[1].texCoord = {1.0f, 0.0f};
                        vertices_to_add[2].texCoord = {0.0f, 0.0f};
                        vertices_to_add[3].texCoord = {0.0f, 1.0f};

                        InsertFace(vertices, vertices_to_add, {}, block, glm::vec2{quad.w, quad.h});
                    }
                }
            }
        }

        GenerateIndices(indices, vertices.size() / 4);

        Engine::Allocator::DeallocateArray(face_layers);
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

    void ChunkMesh::GenerateVertexDataStupid(const BlockData* blockData, std::vector<VoxelVertex>& vertices,
                                             std::vector<uint32_t>& indices, uint32_t& maxIndex)
    {
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            for (size_t x = 0; x < CHUNK_SIZE; x++)
            {
                for (size_t z = 0; z < CHUNK_SIZE; z++)
                {
                    glm::vec3 topPosition = glm::vec3{x, y + 1, z};
                    glm::vec3 bottomPosition = glm::vec3{x, y - 1, z};
                    glm::vec3 leftPosition = glm::vec3{x - 1, y, z};
                    glm::vec3 rightPosition = glm::vec3{x + 1, y, z};
                    glm::vec3 frontPosition = glm::vec3{x, y, z + 1};
                    glm::vec3 backPosition = glm::vec3{x, y, z - 1};
                    if (!blockData->IsSolidSafe(topPosition))
                    {
                        InsertFaceStupid(vertices, TopBlockFaceVertices, glm::vec3{x, y, z});
                    }
                    if (!blockData->IsSolidSafe(bottomPosition))
                    {
                        InsertFaceStupid(vertices, BottomBlockFaceVertices, glm::vec3{x, y, z});
                    }
                    if (!blockData->IsSolidSafe(leftPosition))
                    {
                        InsertFaceStupid(vertices, LeftBlockFaceVertices, glm::vec3{x, y, z});
                    }
                    if (!blockData->IsSolidSafe(rightPosition))
                    {
                        InsertFaceStupid(vertices, RightBlockFaceVertices, glm::vec3{x, y, z});
                    }
                    if (!blockData->IsSolidSafe(frontPosition))
                    {
                        InsertFaceStupid(vertices, FrontBlockFaceVertices, glm::vec3{x, y, z});
                    }
                    if (!blockData->IsSolidSafe(backPosition))
                    {
                        InsertFaceStupid(vertices, BackBlockFaceVertices, glm::vec3{x, y, z});
                    }
                }
            }
        }
        GenerateIndices(indices, vertices.size() / 4);
    }

    void ChunkMesh::InsertFaceStupid(std::vector<VoxelVertex>& vertices, std::array<VoxelVertex, 4> verticesToAdd,
                                     glm::vec3 position, uint8_t block)
    {
        std::for_each(verticesToAdd.begin(), verticesToAdd.end(), [block](VoxelVertex& vertex) {
            vertex.compressedData = 257;
            vertex.compressedData |= ((uint32_t) block & 0xFF) << 16;
        });
        vertices.insert(vertices.end(), verticesToAdd.begin(), verticesToAdd.end());
    }

    void ChunkMesh::InsertFace(std::vector<VoxelVertex>& vertices, std::array<VoxelVertex, 4>& verticesToAdd,
                               glm::vec3 position, uint8_t block, glm::vec2 tiling, glm::vec3 scale)
    {
        std::for_each(verticesToAdd.begin(), verticesToAdd.end(),
                      [&position, &scale, &tiling, &block](VoxelVertex& vertex) {
                          vertex.pos = (vertex.pos * scale) + position;
                          vertex.compressedData = (uint32_t) tiling.x & 0xFF;
                          vertex.compressedData |= ((uint32_t) tiling.y & 0xFF) << 8;
                          vertex.compressedData |= ((uint32_t) block & 0xFF) << 16;
                      });

        vertices.insert(vertices.end(), verticesToAdd.begin(), verticesToAdd.end());
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
        if (axis == 0)
        {
            vertices[0].pos = glm::vec3{quad.x, z, quad.y + quad.h};
            vertices[1].pos = glm::vec3{quad.x, z, quad.y};
            vertices[2].pos = glm::vec3{quad.x + quad.w, z, quad.y};
            vertices[3].pos = glm::vec3{quad.x + quad.w, z, quad.y + quad.h};
        }
        else if (axis == 1)
        {

            vertices[0].pos = glm::vec3{quad.x, z + 1, quad.y + quad.h};
            vertices[1].pos = glm::vec3{quad.x, z + 1, quad.y};
            vertices[2].pos = glm::vec3{quad.x + quad.w, z + 1, quad.y};
            vertices[3].pos = glm::vec3{quad.x + quad.w, z + 1, quad.y + quad.h};
        }
        else if (axis == 2)
        {
            vertices[0].pos = glm::vec3{z, quad.y, quad.x};
            vertices[1].pos = glm::vec3{z, quad.y + quad.h, quad.x};
            vertices[2].pos = glm::vec3{z, quad.y + quad.h, quad.x + quad.w};
            vertices[3].pos = glm::vec3{z, quad.y, quad.x + quad.w};
        }
        else if (axis == 3)
        {
            vertices[0].pos = glm::vec3{z + 1, quad.y, quad.x};
            vertices[1].pos = glm::vec3{z + 1, quad.y + quad.h, quad.x};
            vertices[2].pos = glm::vec3{z + 1, quad.y + quad.h, quad.x + quad.w};
            vertices[3].pos = glm::vec3{z + 1, quad.y, quad.x + quad.w};
        }
        else if (axis == 5)
        {
            vertices[0].pos = glm::vec3{quad.x, quad.y + quad.h, z + 1};
            vertices[1].pos = glm::vec3{quad.x, quad.y, z + 1};
            vertices[2].pos = glm::vec3{quad.x + quad.w, quad.y, z + 1};
            vertices[3].pos = glm::vec3{quad.x + quad.w, quad.y + quad.h, z + 1};
        }
        else if (axis == 4)
        {
            vertices[0].pos = glm::vec3{quad.x, quad.y + quad.h, z};
            vertices[1].pos = glm::vec3{quad.x, quad.y, z};
            vertices[2].pos = glm::vec3{quad.x + quad.w, quad.y, z};
            vertices[3].pos = glm::vec3{quad.x + quad.w, quad.y + quad.h, z};
        }
        vertices[0].axis = axis;
        vertices[1].axis = axis;
        vertices[2].axis = axis;
        vertices[3].axis = axis;
        return vertices;
    }

    void ChunkMesh::Destroy()
    {
        m_VertexArray->Destroy();
        Engine::Allocator::Deallocate(m_VertexArray);
        m_VertexArray = nullptr;
    }
}// namespace Engine
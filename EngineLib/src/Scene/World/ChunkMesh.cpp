#include "ChunkMesh.hpp"
#include "BlockVertexData.hpp"

#include <algorithm>
#include <bitset>
#include <future>
#include <unordered_map>
#include <Core/Timer.hpp>

void ChunkMesh::Generate(const TerrainShapeData* chunkData)
{
    ScopedTimer timer("ChunkMesh::Generate");
    std::vector<Engine::Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t maxIndex = 0;

    GenerateVertexDataStupid(chunkData, vertices, indices, maxIndex);

    m_VertexArray = Engine::VertexArray::Create(indices.size());
    m_VertexArray->Bind();
    m_VertexArray->AddVertexBuffer(
            Engine::VertexBuffer::Create(Engine::Vertex::GetLayout(), (float*) vertices.data(), vertices.size()));
    m_VertexArray->AddIndexBuffer(Engine::IndexBuffer::Create(indices.data(), indices.size()));
}

void ChunkMesh::GenerateFaceLayer(const TerrainShapeData* chunkData, uint32_t axis, uint32_t** data)
{
    switch (axis)
    {
        case 0:
            for (size_t y = 0; y < CHUNK_SIZE; y++)
            {
                for (size_t z = 0; z < CHUNK_SIZE; z++)
                {
                    for (size_t x = 0; x < CHUNK_SIZE; x++)
                    {
                        if (chunkData->IsSolid(glm::ivec3(x, y, z)))
                        {
                            (*data)[z * CHUNK_SIZE + x] |= 1 << (uint32_t) y;
                        }
                    }
                }
            }
            break;
        case 1:

            for (size_t x = 0; x < CHUNK_SIZE; x++)
            {
                for (size_t y = 0; y < CHUNK_SIZE; y++)
                {
                    for (size_t z = 0; z < CHUNK_SIZE; z++)
                    {
                        if (chunkData->IsSolid(glm::ivec3(x, y, z)))
                        {
                            (*data)[y * CHUNK_SIZE + z] |= 1 << (uint32_t) x;
                        }
                    }
                }
            }
            break;
        case 2:

            for (size_t z = 0; z < CHUNK_SIZE; z++)
            {
                for (size_t y = 0; y < CHUNK_SIZE; y++)
                {
                    for (size_t x = 0; x < CHUNK_SIZE; x++)
                    {
                        if (chunkData->IsSolid(glm::ivec3(x, y, z)))
                        {
                            (*data)[y * CHUNK_SIZE + x] |= 1 << (uint32_t) z;
                        }
                    }
                }
            }
            break;
    }
}

void ChunkMesh::GenerateVertexData(const TerrainShapeData* chunkData, std::vector<Engine::Vertex>& vertices,
                                   std::vector<uint32_t>& indices, uint32_t& maxIndex)
{
    uint32_t* face_layers = Engine::Allocator::AllocateArray<uint32_t>(CHUNK_SIZE_CUBIC * 3);
    uint32_t* face_layers2 = &face_layers[CHUNK_SIZE_CUBIC];
    uint32_t* face_layers3 = &face_layers2[CHUNK_SIZE_CUBIC];
    auto task0 = std::async(std::launch::async, ChunkMesh::GenerateFaceLayer, chunkData, (uint32_t) 0, &face_layers);
    auto task1 = std::async(std::launch::async, ChunkMesh::GenerateFaceLayer, chunkData, (uint32_t) 1, &face_layers2);
    auto task2 = std::async(std::launch::async, ChunkMesh::GenerateFaceLayer, chunkData, (uint32_t) 2, &face_layers3);
    task0.get();
    task1.get();
    task2.get();

    std::unordered_map<uint32_t, std::vector<uint32_t>> col_face_map;
    col_face_map[0].resize(CHUNK_SIZE_SQUARE);
    col_face_map[1].resize(CHUNK_SIZE_SQUARE);
    col_face_map[2].resize(CHUNK_SIZE_SQUARE);
    col_face_map[3].resize(CHUNK_SIZE_SQUARE);
    col_face_map[4].resize(CHUNK_SIZE_SQUARE);
    col_face_map[5].resize(CHUNK_SIZE_SQUARE);

    for (uint32_t axis = 0; axis < 3; axis++)
    {
        for (size_t y = 0; y < CHUNK_SIZE; y++)
        {
            auto layer = &face_layers[axis * CHUNK_SIZE_CUBIC];

            for (size_t x = 0; x < CHUNK_SIZE; x++)
            {
                uint32_t current_row = layer[y * CHUNK_SIZE + x];
                col_face_map[axis * 2][y * CHUNK_SIZE + x] = current_row & ~(current_row >> 1);
                col_face_map[axis * 2 + 1][y * CHUNK_SIZE + x] = current_row & ~(current_row >> 1);
            }
        }
    }

    std::vector<std::vector<std::vector<uint32_t>>> data;
    //data[axis][y] - face
    data.resize(6);
    for (uint32_t axis = 0; axis < 6; axis++)
    {

        for (uint32_t z = 0; z < CHUNK_SIZE; z++)
        {
            for (uint32_t x = 0; x < CHUNK_SIZE; x++)
            {
                uint32_t col = col_face_map[axis][z * CHUNK_SIZE + x];
                while (col != 0)
                {
                    uint32_t y = std::countr_zero(col);
                    col &= col - 1;
                    glm::vec3 voxel_pos;
                    switch (axis)
                    {
                        case 0:
                        case 1:
                            voxel_pos = glm::vec3(x, y, z);
                            break;

                        case 2:
                        case 3:
                            voxel_pos = glm::vec3(y, z, x);
                            break;
                        default:
                            voxel_pos = glm::vec3(x, z, y);
                    }
                    if (data[axis].size() < CHUNK_SIZE) data[axis].resize(CHUNK_SIZE);
                    auto& buff = data[axis][y];

                    if (data[axis][y].size() < CHUNK_SIZE) data[axis][y].resize(CHUNK_SIZE);
                    buff[x] |= 1 << (uint32_t) z;

                    LOG("");
                }
            }
        }
    }

    std::vector<std::vector<Quad>*> greedy_quads;
    greedy_quads.resize(6);
    for (uint32_t currentAxis = 0; currentAxis < 6; currentAxis++)
    {
        for (uint32_t y = 0; y < CHUNK_SIZE; y++)
        {
            auto& plane = data[currentAxis][y];
            if (plane.size() > 0)
            {
                greedy_quads[currentAxis] = BinaryGreedyMesherPlane(&plane[0], currentAxis);

                for (const Quad& quad: *greedy_quads[currentAxis])
                {
                    std::array<Engine::Vertex, 4> vertices_to_add;
                    vertices_to_add[0] = GetVertex(currentAxis, quad.x, quad.y, y);
                    vertices_to_add[1] = GetVertex(currentAxis, quad.x + quad.w, quad.y, y);
                    vertices_to_add[2] = GetVertex(currentAxis, quad.x + quad.w, quad.y + quad.h, y);
                    vertices_to_add[3] = GetVertex(currentAxis, quad.x, quad.y + quad.h, y);

                    vertices_to_add[0].texCoord = {0, 1};
                    vertices_to_add[1].texCoord = {0, 0};
                    vertices_to_add[2].texCoord = {1, 0};
                    vertices_to_add[3].texCoord = {1, 1};

                    InsertFace(vertices, indices, maxIndex, vertices_to_add, {});
                }
            }
        }
    }
    Engine::Allocator::DeallocateArray(face_layers);
}

std::vector<Quad>* ChunkMesh::BinaryGreedyMesherPlane(uint32_t* faceData, uint32_t axis)
{
    std::vector<Quad>* quads = new std::vector<Quad>();
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
            quads->push_back({.x = x, .y = y, .w = w, .h = h});
            y += h;
        }
    }
    return quads;
}

void ChunkMesh::GenerateVertexDataStupid(const TerrainShapeData* chunkData, std::vector<Engine::Vertex>& vertices,
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
                if (!chunkData->IsSolidSafe(topPosition))
                {
                    InsertFace(vertices, indices, maxIndex, TopBlockFaceVertices, glm::vec3{x, y, z});
                }
                if (!chunkData->IsSolidSafe(bottomPosition))
                {
                    InsertFace(vertices, indices, maxIndex, BottomBlockFaceVertices, glm::vec3{x, y, z});
                }
                if (!chunkData->IsSolidSafe(leftPosition))
                {
                    InsertFace(vertices, indices, maxIndex, LeftBlockFaceVertices, glm::vec3{x, y, z});
                }
                if (!chunkData->IsSolidSafe(rightPosition))
                {
                    InsertFace(vertices, indices, maxIndex, RightBlockFaceVertices, glm::vec3{x, y, z});
                }
                if (!chunkData->IsSolidSafe(frontPosition))
                {
                    InsertFace(vertices, indices, maxIndex, FrontBlockFaceVertices, glm::vec3{x, y, z});
                }
                if (!chunkData->IsSolidSafe(backPosition))
                {
                    InsertFace(vertices, indices, maxIndex, BackBlockFaceVertices, glm::vec3{x, y, z});
                }
            }
        }
    }
}

void ChunkMesh::InsertFace(std::vector<Engine::Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t& maxIndex,
                           const std::array<Engine::Vertex, 4>& verticesToAdd, glm::vec3 position, glm::vec3 scale)
{
    if (position != glm::vec3{0, 0, 0})
    {

        std::vector<Engine::Vertex> transformedVertices(verticesToAdd.begin(), verticesToAdd.end());
        std::for_each(transformedVertices.begin(), transformedVertices.end(),
                      [position, scale](Engine::Vertex& vertex) { vertex.pos = (vertex.pos * scale) + position; });

        vertices.insert(vertices.end(), transformedVertices.begin(), transformedVertices.end());
    }
    else { vertices.insert(vertices.end(), verticesToAdd.begin(), verticesToAdd.end()); }
    constexpr uint32_t numIndices = 6;
    uint32_t indices_array[numIndices] = {maxIndex + 0, maxIndex + 1, maxIndex + 2,
                                          maxIndex + 0, maxIndex + 2, maxIndex + 3};
    indices.insert(indices.end(), indices_array, indices_array + numIndices);
    maxIndex += 4;
}

Engine::Vertex ChunkMesh::GetVertex(uint32_t axis, uint32_t x, uint32_t y, uint32_t z)
{

    switch (axis)
    {
        case 0:
            return Engine::Vertex{glm::vec3(axis, y, x), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)};
        case 1:
            return Engine::Vertex{glm::vec3(z + 1, y, x), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)};
        case 2:
            return Engine::Vertex{glm::vec3(x, 0, y), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f)};
        case 3:
            return Engine::Vertex{glm::vec3(x, z + 1, y), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)};
        case 4:
            return Engine::Vertex{glm::vec3(x, y, 0), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f)};
        case 5:
            return Engine::Vertex{glm::vec3(x, y, z + 1), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f)};
        default:
            return {};
    }
}

void ChunkMesh::Destroy()
{
    m_VertexArray->Destroy();
    Engine::Allocator::Deallocate(m_VertexArray);
    m_VertexArray = nullptr;
}

#include "ChunkMesh.hpp"
#include "BlockVertexData.hpp"

#include <algorithm>

#include <Core/Timer.hpp>

void ChunkMesh::Generate(const TerrainShapeData* chunkData)
{
    ScopedTimer timer("ChunkMesh::Generate");
    std::vector<Engine::Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t maxIndex = 0;

    GenerateVertexData(chunkData, vertices, indices, maxIndex);

    m_VertexArray = Engine::VertexArray::Create(indices.size());
    m_VertexArray->Bind();
    m_VertexArray->AddVertexBuffer(
            Engine::VertexBuffer::Create(Engine::Vertex::GetLayout(), (float*) vertices.data(), vertices.size()));
    m_VertexArray->AddIndexBuffer(Engine::IndexBuffer::Create(indices.data(), indices.size()));
}

void ChunkMesh::GenerateVertexData(const TerrainShapeData* chunkData, std::vector<Engine::Vertex>& vertices,
                                   std::vector<uint32_t>& indices, uint32_t& maxIndex)
{
    
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
    std::vector<Engine::Vertex> transformedVertices(verticesToAdd.begin(), verticesToAdd.end());
    std::for_each(transformedVertices.begin(), transformedVertices.end(),
                  [position, scale](Engine::Vertex& vertex) { vertex.pos = (vertex.pos * scale) + position; });

    vertices.insert(vertices.end(), transformedVertices.begin(), transformedVertices.end());

    constexpr uint32_t numIndices = 6;
    uint32_t indices_array[numIndices] = {maxIndex + 0, maxIndex + 1, maxIndex + 2,
                                          maxIndex + 2, maxIndex + 3, maxIndex + 0};
    indices.insert(indices.end(), indices_array, indices_array + numIndices);
    maxIndex += 4;
}

void ChunkMesh::Destroy()
{
    m_VertexArray->Destroy();
    delete m_VertexArray;
    m_VertexArray = nullptr;
}

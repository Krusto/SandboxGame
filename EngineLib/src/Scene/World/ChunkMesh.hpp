#pragma once
#include "ChunkData.hpp"
#include <vector>
#include <Renderer/Vertex.hpp>
#include <Renderer/VertexArray.hpp>

class ChunkMesh
{
public:
    ChunkMesh() = default;
    ~ChunkMesh() = default;

public:
    void Destroy();
    void Generate(const TerrainShapeData* chunkData);

    const Engine::VertexArray* GetVertexArray() const { return m_VertexArray; }

private:
    void GenerateVertexData(const TerrainShapeData* chunkData, std::vector<Engine::Vertex>& vertices,
                            std::vector<uint32_t>& indices, uint32_t& maxIndex);
    void GenerateVertexDataStupid(const TerrainShapeData* chunkData, std::vector<Engine::Vertex>& vertices,
                                  std::vector<uint32_t>& indices, uint32_t& maxIndex);

    void InsertFace(std::vector<Engine::Vertex>& vertices, std::vector<uint32_t>& indices, uint32_t& maxIndex,
                    const std::array<Engine::Vertex, 4>& verticesToAdd, glm::vec3 position,
                    glm::vec3 scale = {1, 1, 1});


    Engine::VertexArray* m_VertexArray;
};

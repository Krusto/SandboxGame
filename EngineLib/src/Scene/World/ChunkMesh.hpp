#pragma once
#include "ChunkData.hpp"
#include <vector>
#include <array>
#include <Renderer/VertexArray.hpp>
#include "VoxelVertex.hpp"

struct Quad {
    uint64_t x, y, w, h;
};

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
private:
    void GenerateVertexData(const TerrainShapeData* chunkData, std::vector<VoxelVertex>& vertices,
                            std::vector<uint32_t>& indices, uint32_t& maxIndex);

    std::vector<Quad> BinaryGreedyMesherPlane(uint32_t* faceData, uint32_t axis);

    void GenerateVertexDataStupid(const TerrainShapeData* chunkData, std::vector<VoxelVertex>& vertices,
                                  std::vector<uint32_t>& indices, uint32_t& maxIndex);

    void InsertFace(std::vector<VoxelVertex>& vertices, std::vector<uint32_t>& indices, uint32_t& maxIndex,
                    const std::array<VoxelVertex, 4>& verticesToAdd, glm::vec3 position, glm::vec3 scale = {1, 1, 1});

    std::array<VoxelVertex, 4> GetVertex(uint32_t axis, const Quad& quad, uint32_t z);

private:
    static void GenerateFaceLayer(const TerrainShapeData* chunkData, uint32_t axis, uint32_t* data);

private:
    Engine::VertexArray* m_VertexArray;
};

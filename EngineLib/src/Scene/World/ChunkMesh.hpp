#pragma once
#include <vector>
#include <array>
#include <Renderer/VertexArray.hpp>

#include "BlockRegistry.hpp"
#include "BlockData.hpp"
#include "VoxelVertex.hpp"

namespace Engine
{

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
        void Generate(const BlockData* blockData);

        const Engine::VertexArray* GetVertexArray() const { return m_VertexArray; }

    private:
    private:
        void GenerateVertexData(const BlockData* blockData, std::vector<VoxelVertex>& vertices,
                                std::vector<uint32_t>& indices, uint32_t& maxIndex);

        std::vector<Quad> BinaryGreedyMesherPlane(uint32_t* faceData, uint32_t axis);

        void GenerateVertexDataStupid(const BlockData* blockData, std::vector<VoxelVertex>& vertices,
                                      std::vector<uint32_t>& indices, uint32_t& maxIndex);

        void InsertFaceStupid(std::vector<VoxelVertex>& vertices, std::array<VoxelVertex, 4> verticesToAdd,
                              glm::vec3 position, uint8_t block = BlockType::GRASS);

        void InsertFace(std::vector<VoxelVertex>& vertices, std::array<VoxelVertex, 4>& verticesToAdd,
                        glm::vec3 position, uint8_t block = BlockType::GRASS, glm::vec2 tiling = {1, 1},
                        glm::vec3 scale = {1, 1, 1});

        void GenerateIndices(std::vector<uint32_t>& indices, uint32_t numQuads);

        std::array<VoxelVertex, 4> GetVertices(uint32_t axis, const Quad& quad, uint32_t z);

    private:
        static void GenerateFaceLayer(const BlockData* blockData, uint32_t axis, uint32_t* data);

    private:
        Engine::VertexArray* m_VertexArray;
    };
}// namespace Engine
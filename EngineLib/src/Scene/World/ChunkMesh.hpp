#pragma once
#include <vector>
#include <array>
#include <Renderer/Renderer.hpp>

#include "BlockRegistry.hpp"
#include "BlockData.hpp"
#include "VoxelVertex.hpp"

namespace Engine
{

    struct Quad {
        uint64_t x, y, w, h;
    };

    struct ChunkMeshRaw {
        std::vector<VoxelVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<uint8_t> blocks;
        uint64_t* opaqueMask;
        uint64_t* faceMasks;
        uint8_t* forwardMerged;
        uint8_t* rightMerged;
    };

    class ChunkMesh
    {
    public:
        ChunkMesh() = default;
        ~ChunkMesh() = default;

    public:
        void Destroy();
        void Generate(const BlockData* blockData);

        const Engine::VertexArray GetVertexArray() const { return m_VertexArray; }

        const Engine::StorageBuffer* GetBlocksBuffer() const { return &m_BlocksBuffer; }

        const Engine::StorageBuffer* GetQuadsBuffer() const { return &m_QuadsBuffer; }

        static void UploadData(ChunkMesh* mesh);

    private:
        void GenerateVertexData(const BlockData* blockData, std::vector<uint8_t>& blocks,
                                std::vector<VoxelVertex>& vertices, std::vector<uint32_t>& indices);

        void GenerateVertexData2(const BlockData* blockData, std::vector<uint8_t>& blocks,
                                 std::vector<VoxelVertex>& vertices, std::vector<uint32_t>& indices);

        static std::vector<Quad> BinaryGreedyMesherPlane(uint32_t* faceData, uint32_t axis);

        static void InsertFace(std::vector<VoxelVertex>& vertices, uint32_t axis, const Quad& quad, uint32_t z);

        void GenerateIndices(std::vector<uint32_t>& indices, uint32_t numQuads);

        static std::array<VoxelVertex, 4> GetVertices(uint32_t axis, const Quad& quad, uint32_t z);

    private:
        static void GenerateFaceLayer(const BlockData* blockData, uint32_t axis, uint32_t* data);

        static RendererCommand GetGenerateCommand(ChunkMesh* mesh);

    private:
        ChunkMeshRaw m_Mesh{};

        VertexArray m_VertexArray{};
        StorageBuffer m_QuadsBuffer{};
        StorageBuffer m_BlocksBuffer{};
    };
}// namespace Engine
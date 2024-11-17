#pragma once


#include "Chunk.hpp"
#include "BlockData.hpp"
#include "TerrainShape.hpp"
#include "ChunkMesh.hpp"
#include "TerrainGenerator.hpp"

#include <glm/gtx/hash.hpp>

#include <future>
#include <unordered_map>

namespace Engine
{

    enum class ChunkStatus
    {
        None,
        StartGeneratingShape,
        GeneratingShape,
        GeneratingShapeDone,
        StartGeneratingBlockData,
        GeneratingBlockData,
        GeneratingBlockDataDone,
        StartGeneratingMesh,
        GeneratingMesh,
        GeneratingMeshDone
    };

    class ChunkFactory
    {
    public:
        ChunkFactory() = default;
        ~ChunkFactory() = default;

    public:
        static void Init(TerrainGenerationSettings settings);

        static void ScheduleChunkForGeneration(glm::ivec3 chunkPosition);

        static void RunWorkers();

        static void Update();

        static std::unordered_map<glm::ivec3, Chunk> GetChunks();

    public:
        static Chunk GenerateChunk(TerrainGenerationSettings settings, glm::ivec3 chunkPosition);
        static void DestroyChunk(Chunk chunk);

        static BlockData* GenerateBlockData(TerrainGenerationSettings settings, TerrainShape* shapeData,
                                            glm::ivec3 chunkPosition);
        static void DestroyBlockData(BlockData* data);

        static TerrainShape* GenerateTerrainShape(TerrainGenerationSettings settings, glm::ivec3 chunkPosition);
        static void DestroyTerrainShape(TerrainShape* data);

        static ChunkMesh* GenerateChunkMesh(BlockData* blockData);
        static void DestroyChunkMesh(ChunkMesh* mesh);

    private:
        inline static TerrainGenerationSettings s_TerrainGenerationSettings{};

        inline static std::unordered_map<glm::ivec3, ChunkStatus> s_ChunksToGenerate{};

        inline static std::unordered_map<glm::ivec3, std::future<TerrainShape*>> s_TerrainShapeWorkers{};
        inline static std::unordered_map<glm::ivec3, std::future<BlockData*>> s_BlockDataWorkers{};
        inline static std::unordered_map<glm::ivec3, std::future<ChunkMesh*>> s_ChunkMeshWorkers{};
        inline static std::unordered_map<glm::ivec3, Chunk> s_ChunksInProgress{};

        inline static std::unordered_map<glm::ivec3, ChunkStatus> s_ChunkGenerationStatus{};
    };
}// namespace Engine

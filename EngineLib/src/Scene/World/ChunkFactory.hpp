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
        Uploaded,
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
        //static void ScheduleChunkMeshForGeneration(glm::ivec3 chunkPosition);

        static void RunWorkers();

        static void Update();

        static void Reload();

        static std::unordered_map<glm::ivec3, Chunk>& GetGeneratedChunks();

        staticstd::size_t GeneratedChunksCount();

        static void UploadChunks();

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

        inline static std::unordered_map<glm::ivec3, std::future<Chunk*>> s_ChunkGenerationWorkers{};
        inline static std::unordered_map<glm::ivec3, Chunk> s_DoneChunks{};
        inline static std::unordered_map<glm::ivec3, ChunkStatus> s_ScheduledChunks{};
        inline static std::unordered_map<glm::ivec3, ChunkStatus> s_ChunksToUpload{};

        inline staticstd::size_t s_CurrentChunkGenerationCount = 0;
        inline static constexprstd::size_t s_MaxChunksToGenerate = 200;
    };
}// namespace Engine

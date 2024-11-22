#include "ChunkFactory.hpp"
#include <Core/Allocator.hpp>

namespace Engine
{


    void ChunkFactory::Init(TerrainGenerationSettings settings) { s_TerrainGenerationSettings = settings; }

    void ChunkFactory::ScheduleChunkForGeneration(glm::ivec3 chunkPosition)
    {
        s_ScheduledChunks.try_emplace(chunkPosition, ChunkStatus::None);
    }

    void ChunkFactory::RunWorkers() {}

    std::unordered_map<glm::ivec3, Chunk>& ChunkFactory::GetGeneratedChunks()
    {
        LOG_INFO("Getting chunks!\n");
        LOG_INFO("Got %zi!\n", s_DoneChunks.size());

        return s_DoneChunks;
    }

    size_t ChunkFactory::GeneratedChunksCount() { return s_DoneChunks.size(); }

    void ChunkFactory::UploadChunks()
    {
        if (s_ChunksToUpload.size() > 0)
        {
            std::vector<glm::ivec3> chunksToErase;
            chunksToErase.reserve(s_ChunksToUpload.size());
            for (auto& [pos, status]: s_ChunksToUpload)
            {
                ChunkMesh::UploadData(s_DoneChunks[pos].mesh);
                status = ChunkStatus::Uploaded;
                chunksToErase.push_back(pos);
            }
            for (auto& pos: chunksToErase) { s_ChunksToUpload.erase(pos); }
        }
    }

    void ChunkFactory::Update()
    {
        if (!s_ScheduledChunks.empty() && s_CurrentChunkGenerationCount < s_MaxChunksToGenerate)
        {
            for (auto& [chunkPos, status]: s_ScheduledChunks)
            {
                if (s_CurrentChunkGenerationCount >= s_MaxChunksToGenerate ||
                    s_ChunkGenerationWorkers.contains(chunkPos))
                {
                    break;
                }

                s_CurrentChunkGenerationCount++;

                s_ChunkGenerationWorkers[chunkPos] =
                        std::async(std::launch::async, [settings = s_TerrainGenerationSettings, chunkPos]() {
                            Chunk* chunk = Engine::Allocator::Allocate < Chunk>();
                            chunk->terrainShape = GenerateTerrainShape(settings, chunkPos);
                            chunk->blockData = GenerateBlockData(settings, chunk->terrainShape, chunkPos);
                            chunk->mesh = GenerateChunkMesh(chunk->blockData);
                            return chunk;
                        });
            }
        }

        std::vector<glm::ivec3> workersToErase;
        for (auto& [chunkPos, worker]: s_ChunkGenerationWorkers)
        {
            if (worker.valid() && worker.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready)
            {
                Chunk* chunk = worker.get();
                s_CurrentChunkGenerationCount--;
                s_DoneChunks.try_emplace(chunkPos, *chunk);
                Engine::Allocator::Deallocate(chunk);
                s_ChunksToUpload.try_emplace(chunkPos, ChunkStatus::None);
                workersToErase.push_back(chunkPos);
            }
            if (s_ScheduledChunks.contains(chunkPos)) { s_ScheduledChunks.erase(chunkPos); }
        }
        for (auto& chunkPos: workersToErase) { s_ChunkGenerationWorkers.erase(chunkPos); }
    }

    void ChunkFactory::Reload()
    {
        s_CurrentChunkGenerationCount = 0;

        for (auto& [chunkPos, worker]: s_ChunkGenerationWorkers)
        {
            worker.wait();
            auto chunk = worker.get();
            ChunkFactory::DestroyChunk(*chunk);
            delete chunk;
        }
        s_ChunkGenerationWorkers.clear();
        s_ScheduledChunks.clear();

        for (auto& [pos, chunk]: s_DoneChunks) { DestroyChunk(chunk); }
        s_DoneChunks.clear();
    }

    Chunk ChunkFactory::GenerateChunk(TerrainGenerationSettings settings, glm::ivec3 chunkPosition)
    {

        Chunk chunk;

        chunk.terrainShape = GenerateTerrainShape(settings, chunkPosition);

        chunk.blockData = GenerateBlockData(settings, chunk.terrainShape, chunkPosition);

        chunk.mesh = GenerateChunkMesh(chunk.blockData);

        return chunk;
    }

    void ChunkFactory::DestroyChunk(Chunk chunk)
    {

        DestroyTerrainShape(chunk.terrainShape);
        chunk.terrainShape = nullptr;
        DestroyBlockData(chunk.blockData);
        chunk.blockData = nullptr;
        DestroyChunkMesh(chunk.mesh);
        chunk.mesh = nullptr;
    }

    BlockData* ChunkFactory::GenerateBlockData(TerrainGenerationSettings settings, TerrainShape* shapeData,
                                               glm::ivec3 chunkPosition)
    {
        BlockData* ptr =  
        Engine::Allocator::Allocate < BlockData>();
        ptr->Init(settings.Seed);
        TerrainGenerator::GenerateBlocks(settings, shapeData, ptr, chunkPosition);
        return ptr;
    }

    void ChunkFactory::DestroyBlockData(BlockData* data)
    {
        if (data)
        {
            data->Destroy();
            Engine::Allocator::Deallocate(data);
        }
    }

    TerrainShape* ChunkFactory::GenerateTerrainShape(TerrainGenerationSettings settings, glm::ivec3 chunkPosition)
    {
        TerrainShape* ptr =  
        Engine::Allocator::Allocate < TerrainShape>();
        TerrainGenerator::GenerateTerrainShape(settings, ptr, chunkPosition);
        return ptr;
    }

    void ChunkFactory::DestroyTerrainShape(TerrainShape* data)
    {
        if (data)
        {
            data->Destroy();
            Engine::Allocator::Deallocate(data);
        }
    }

    ChunkMesh* ChunkFactory::GenerateChunkMesh(BlockData* blockData)
    {
        ChunkMesh* ptr = Engine::Allocator::Allocate < ChunkMesh>();
        ptr->Generate(blockData);
        return ptr;
    }

    void ChunkFactory::DestroyChunkMesh(ChunkMesh* mesh)
    {
        if (mesh)
        {
            mesh->Destroy();
            Engine::Allocator::Deallocate(mesh);
        }
    }
}// namespace Engine

#include "ChunkFactory.hpp"
#include <Core/Allocator.hpp>

namespace Engine
{


    void ChunkFactory::Init(TerrainGenerationSettings settings) { s_TerrainGenerationSettings = settings; }

    void ChunkFactory::ScheduleChunkForGeneration(glm::ivec3 chunkPosition)
    {
        s_ChunksToGenerate.try_emplace(chunkPosition, ChunkStatus::None);
    }

    void ChunkFactory::RunWorkers() {}

    std::unordered_map<glm::ivec3, Chunk> ChunkFactory::GetChunks()
    {
        std::unordered_map<glm::ivec3, Chunk> chunks;
        for (auto& [chunkPos, chunk]: s_ChunksInProgress)
        {
            if (s_ChunkGenerationStatus[chunkPos] == ChunkStatus::GeneratingMeshDone)
            {
                chunks.try_emplace(chunkPos, chunk);
            }
        }
        for (auto& [chunkPos, chunk]: chunks) { s_ChunksInProgress.erase(chunkPos); }
        return chunks;
    }

    void ChunkFactory::Update()
    {
        if (!s_ChunksToGenerate.empty())
        {
            for (auto& [chunkPos, status]: s_ChunksToGenerate)
            {
                s_ChunkGenerationStatus.try_emplace(chunkPos, status);
            }
            s_ChunksToGenerate.clear();
        }

        for (auto& [chunkPos, status]: s_ChunkGenerationStatus)
        {
            switch (status)
            {
                case ChunkStatus::None: {
                    status = ChunkStatus::StartGeneratingShape;
                }
                break;
                case ChunkStatus::StartGeneratingShape: {
                    if (!s_TerrainShapeWorkers.contains(chunkPos))
                    {
                        s_TerrainShapeWorkers[chunkPos] = std::async(std::launch::async, GenerateTerrainShape,
                                                                     s_TerrainGenerationSettings, chunkPos);
                        status = ChunkStatus::GeneratingShape;
                        LOG("Generating shape for chunk %d %d %d\n", chunkPos.x, chunkPos.y, chunkPos.z);
                    }
                }
                break;
                case ChunkStatus::GeneratingShape: {
                    if (s_TerrainShapeWorkers[chunkPos].valid())
                    {
                        if (s_TerrainShapeWorkers[chunkPos].wait_for(std::chrono::milliseconds(0)) ==
                            std::future_status::ready)
                        {
                            status = ChunkStatus::StartGeneratingBlockData;
                            LOG("Generated shape for chunk %d %d %d\n", chunkPos.x, chunkPos.y, chunkPos.z);
                        }
                    }
                }
                break;
                case ChunkStatus::StartGeneratingBlockData: {
                    if (!s_BlockDataWorkers.contains(chunkPos))
                    {
                        auto ptr = s_TerrainShapeWorkers[chunkPos].get();
                        s_BlockDataWorkers[chunkPos] = std::async(std::launch::async, GenerateBlockData,
                                                                  s_TerrainGenerationSettings, ptr, chunkPos);
                        s_ChunksInProgress[chunkPos].terrainShape = ptr;
                        status = ChunkStatus::GeneratingBlockData;
                        LOG("Generating block data for chunk %d %d %d\n", chunkPos.x, chunkPos.y, chunkPos.z);
                    }
                }
                break;
                case ChunkStatus::GeneratingBlockData: {
                    if (s_BlockDataWorkers[chunkPos].valid())
                    {
                        if (s_BlockDataWorkers[chunkPos].wait_for(std::chrono::milliseconds(0)) ==
                            std::future_status::ready)
                        {
                            DestroyTerrainShape(s_ChunksInProgress[chunkPos].terrainShape);
                            s_ChunksInProgress[chunkPos].terrainShape = nullptr;
                            status = ChunkStatus::StartGeneratingMesh;
                            LOG("Generated block data for chunk %d %d %d\n", chunkPos.x, chunkPos.y, chunkPos.z);
                        }
                    }
                }
                break;
                case ChunkStatus::StartGeneratingMesh: {
                    if (!s_ChunkMeshWorkers.contains(chunkPos))
                    {
                        auto ptr = s_BlockDataWorkers[chunkPos].get();

                        s_ChunkMeshWorkers[chunkPos] = std::async(std::launch::async, GenerateChunkMesh, ptr);

                        s_ChunksInProgress[chunkPos].blockData = ptr;
                        status = ChunkStatus::GeneratingMesh;
                        LOG("Generating mesh for chunk %d %d %d\n", chunkPos.x, chunkPos.y, chunkPos.z);
                    }
                }
                break;
                case ChunkStatus::GeneratingMesh: {
                    if (s_ChunkMeshWorkers[chunkPos].valid())
                    {
                        if (s_ChunkMeshWorkers[chunkPos].wait_for(std::chrono::milliseconds(0)) ==
                            std::future_status::ready)
                        {
                            s_ChunksInProgress[chunkPos].mesh = s_ChunkMeshWorkers[chunkPos].get();
                            status = ChunkStatus::GeneratingMeshDone;
                            LOG("Generated mesh for chunk %d %d %d\n", chunkPos.x, chunkPos.y, chunkPos.z);
                            ChunkMesh::UploadData(s_ChunksInProgress[chunkPos].mesh);
                        }
                    }
                }
                break;
            }
        }
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

        DestroyBlockData(chunk.blockData);

        DestroyChunkMesh(chunk.mesh);
    }

    BlockData* ChunkFactory::GenerateBlockData(TerrainGenerationSettings settings, TerrainShape* shapeData,
                                               glm::ivec3 chunkPosition)
    {
        Allocate(BlockData, ptr);
        ptr->Init(settings.Seed);
        TerrainGenerator::GenerateBlocks(settings, shapeData, ptr, chunkPosition);
        return ptr;
    }

    void ChunkFactory::DestroyBlockData(BlockData* data)
    {
        if (data)
        {
            data->Destroy();
            Deallocate(data);
        }
    }

    TerrainShape* ChunkFactory::GenerateTerrainShape(TerrainGenerationSettings settings, glm::ivec3 chunkPosition)
    {
        Allocate(TerrainShape, ptr);
        TerrainGenerator::GenerateTerrainShape(settings, ptr, chunkPosition);
        return ptr;
    }

    void ChunkFactory::DestroyTerrainShape(TerrainShape* data)
    {
        if (data)
        {
            data->Destroy();
            Deallocate(data);
        }
    }

    ChunkMesh* ChunkFactory::GenerateChunkMesh(BlockData* blockData)
    {
        Allocate(ChunkMesh, ptr);
        ptr->Generate(blockData);
        return ptr;
    }

    void ChunkFactory::DestroyChunkMesh(ChunkMesh* mesh)
    {
        if (mesh)
        {
            mesh->Destroy();
            Deallocate(mesh);
        }
    }
}// namespace Engine

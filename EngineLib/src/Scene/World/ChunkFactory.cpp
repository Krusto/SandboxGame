#include "ChunkFactory.hpp"
#include <Core/Allocator.hpp>

Engine::Chunk Engine::ChunkFactory::GenerateChunk(TerrainGenerationSettings* settings, glm::ivec3 chunkPosition) const
{

    Chunk chunk;

    chunk.terrainShape = GenerateTerrainShape(settings, chunkPosition);

    chunk.blockData = GenerateBlockData(settings, chunk.terrainShape, chunkPosition);

    chunk.mesh = GenerateChunkMesh(chunk.blockData);

    return chunk;
}

void Engine::ChunkFactory::DestroyChunk(Chunk chunk) const
{

    DestroyTerrainShape(chunk.terrainShape);

    DestroyBlockData(chunk.blockData);

    DestroyChunkMesh(chunk.mesh);
}

Engine::BlockData* Engine::ChunkFactory::GenerateBlockData(TerrainGenerationSettings* settings, TerrainShape* shapeData,
                                                           glm::ivec3 chunkPosition)
{
    auto ptr = Allocator::Allocate<BlockData>();
    ptr->Init(settings->Seed);
    TerrainGenerator::GenerateBlocks(*settings, shapeData, ptr, chunkPosition);
    return ptr;
}

void Engine::ChunkFactory::DestroyBlockData(BlockData* data)
{
    data->Destroy();
    Allocator::Deallocate(data);
}

Engine::TerrainShape* Engine::ChunkFactory::GenerateTerrainShape(TerrainGenerationSettings* settings,
                                                                 glm::ivec3 chunkPosition)
{
    auto ptr = Allocator::Allocate<TerrainShape>();
    TerrainGenerator::GenerateTerrainShape(*settings, ptr, chunkPosition);
    return ptr;
}

void Engine::ChunkFactory::DestroyTerrainShape(TerrainShape* data)
{
    data->Destroy();
    Allocator::Deallocate(data);
}

Engine::ChunkMesh* Engine::ChunkFactory::GenerateChunkMesh(BlockData* blockData)
{
    auto ptr = Allocator::Allocate<ChunkMesh>();
    ptr->Generate(blockData);
    return ptr;
}

void Engine::ChunkFactory::DestroyChunkMesh(ChunkMesh* mesh)
{
    mesh->Destroy();
    Allocator::Deallocate(mesh);
}
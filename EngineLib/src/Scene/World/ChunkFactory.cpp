#include "ChunkFactory.hpp"
#include <Core/Allocator.hpp>

void Engine::ChunkFactory::Init(TerrainGenerationSettings settings) { m_Generator.Init(settings); }

Engine::Chunk Engine::ChunkFactory::GenerateChunk(glm::ivec3 chunkPosition) const
{

    Chunk chunk;

    chunk.terrainShape = GenerateTerrainShape(chunkPosition);

    chunk.blockData = GenerateBlockData(chunk.terrainShape, chunkPosition);

    chunk.mesh = GenerateChunkMesh(chunk.blockData);

    return chunk;
}

void Engine::ChunkFactory::DestroyChunk(Chunk chunk) const
{

    DestroyTerrainShape(chunk.terrainShape);

    DestroyBlockData(chunk.blockData);

    DestroyChunkMesh(chunk.mesh);
}

Engine::BlockData* Engine::ChunkFactory::GenerateBlockData(TerrainShape* shapeData, glm::ivec3 chunkPosition) const
{
    auto ptr = Allocator::Allocate<BlockData>();
    ptr->Init(m_Generator.GetSeed());
    m_Generator.GenerateBlocks(shapeData, ptr, chunkPosition);
    return ptr;
}

void Engine::ChunkFactory::DestroyBlockData(BlockData* data) const
{
    data->Destroy();
    Allocator::Deallocate(data);
}

Engine::TerrainShape* Engine::ChunkFactory::GenerateTerrainShape(glm::ivec3 chunkPosition) const
{
    auto ptr = Allocator::Allocate<TerrainShape>();
    m_Generator.GenerateTerrainShape(ptr, chunkPosition);
    return ptr;
}

void Engine::ChunkFactory::DestroyTerrainShape(TerrainShape* data) const
{
    data->Destroy();
    Allocator::Deallocate(data);
}

Engine::ChunkMesh* Engine::ChunkFactory::GenerateChunkMesh(BlockData* blockData) const
{
    auto ptr = Allocator::Allocate<ChunkMesh>();
    ptr->Generate(blockData);
    return ptr;
}

void Engine::ChunkFactory::DestroyChunkMesh(ChunkMesh* mesh) const
{
    mesh->Destroy();
    Allocator::Deallocate(mesh);
}
#include "ChunkFactory.hpp"
#include <Core/Allocator.hpp>

void Engine::ChunkFactory::Init(TerrainGenerationSettings settings) { m_Generator.Init(settings); }

ChunkData* Engine::ChunkFactory::Generate()
{

    auto ptr = Allocator::Allocate<ChunkData>();
    ptr->Init(m_Generator.GetSeed());
    return ptr;
}

void Engine::ChunkFactory::Destroy(ChunkData* data)
{
    data->Destroy();
    Allocator::Deallocate(data);
}

TerrainShapeData* Engine::ChunkFactory::GenerateTerrainShapeData(glm::ivec3 chunkPosition)
{
    auto ptr = Allocator::Allocate<TerrainShapeData>();
    m_Generator.GenerateTerrainShape(ptr, chunkPosition);
    return ptr;
}

void Engine::ChunkFactory::DestroyTerrainShapeData(TerrainShapeData* data)
{
    data->Destroy();
    Allocator::Deallocate(data);
}

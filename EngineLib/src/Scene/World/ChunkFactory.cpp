#include "ChunkFactory.hpp"

ChunkData* Engine::ChunkFactory::Generate(uint32_t seed)
{

    auto ptr = new ChunkData();
    ptr->Init(seed);
    return ptr;
}

void Engine::ChunkFactory::Destroy(ChunkData* data)
{
    data->Destroy();
    delete data;
}

TerrainShapeData* Engine::ChunkFactory::GenerateTerrainShapeData(uint32_t seed)
{
    auto ptr = new TerrainShapeData();
    ptr->Init(seed);
    return ptr;
}

void Engine::ChunkFactory::DestroyTerrainShapeData(TerrainShapeData* data)
{
    data->Destroy();
    delete data;
}

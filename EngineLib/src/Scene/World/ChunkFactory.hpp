#pragma once

#include "ChunkData.hpp"

namespace Engine
{
    class ChunkFactory
    {
    public:
        static ChunkData* Generate(uint32_t seed);
        static void Destroy(ChunkData* data);
        static TerrainShapeData* GenerateTerrainShapeData(uint32_t seed);
        static void DestroyTerrainShapeData(TerrainShapeData* data);
    };
}// namespace Engine

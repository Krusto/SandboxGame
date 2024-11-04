#pragma once

#include "ChunkData.hpp"
#include "TerrainGenerator.hpp"

namespace Engine
{
    class ChunkFactory
    {
    public:
        ChunkFactory() = default;
        ~ChunkFactory() = default;

    public:
        void Init(TerrainGenerationSettings settings);

        ChunkData* Generate();
        void Destroy(ChunkData* data);
        TerrainShapeData* GenerateTerrainShapeData(glm::ivec3 chunkPosition);
        void DestroyTerrainShapeData(TerrainShapeData* data);

    private:
        TerrainGenerator m_Generator;
    };
}// namespace Engine

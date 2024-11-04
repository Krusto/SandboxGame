#include "TerrainGenerator.hpp"

void TerrainGenerator::Init(TerrainGenerationSettings settings) { m_Settings = settings; }

void TerrainGenerator::Destroy() {}

void TerrainGenerator::GenerateTerrainShape(TerrainShapeData* shapeData, glm::ivec3 chunkPosition)
{
    shapeData->Init(m_Settings.Seed);
    // shapeData->Set();
    for (int y = 0; y < CHUNK_SIZE; y++)
    {
        for (int x = 0; x < CHUNK_SIZE; x++)
        {
            for (int z = 0; z < CHUNK_SIZE; z++) { shapeData->Set(glm::ivec3(x, y, z)); }
        }
    }
}

TerrainGenerationSettings TerrainGenerator::GetSettings() const { return m_Settings; }

uint32_t TerrainGenerator::GetSeed() const { return m_Settings.Seed; }

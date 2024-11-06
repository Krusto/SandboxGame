#include "TerrainGenerator.hpp"
#include <PerlinNoise.hpp>

void TerrainGenerator::Init(TerrainGenerationSettings settings) { m_Settings = settings; }

void TerrainGenerator::Destroy() {}

void TerrainGenerator::GenerateTerrainShape(TerrainShapeData* shapeData, glm::ivec3 chunkPosition)
{
    shapeData->Init(m_Settings.Seed);

    const siv::PerlinNoise::seed_type seed = m_Settings.Seed;

    const siv::PerlinNoise perlin{seed};

    for (float z = 0; z < CHUNK_SIZE; z++)
    {
        for (float x = 0; x < CHUNK_SIZE; x++)
        {
            // for (uint32_t y = 0; y < CHUNK_SIZE; y++)
            // {

            //  shapeData->Set(glm::ivec3(x, y, z));
            // }
            float h = perlin.octave2D_01(chunkPosition.x + x / CHUNK_SIZE, chunkPosition.z + z / CHUNK_SIZE, 5) * 10 +
                      15;
            for (int y = 0; y < h; y++) { shapeData->Set(glm::ivec3(x, y, z)); }
        }
    }
}

TerrainGenerationSettings TerrainGenerator::GetSettings() const { return m_Settings; }

uint32_t TerrainGenerator::GetSeed() const { return m_Settings.Seed; }

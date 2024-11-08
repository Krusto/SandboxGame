#include "TerrainGenerator.hpp"
#include <PerlinNoise.hpp>

namespace Engine
{

    void TerrainGenerator::Init(TerrainGenerationSettings settings) { m_Settings = settings; }

    void TerrainGenerator::Destroy() {}

    void TerrainGenerator::GenerateTerrainShape(TerrainShape* shapeData, glm::ivec3 chunkPosition) const
    {
        shapeData->Init(m_Settings.Seed);

        const siv::PerlinNoise::seed_type seed = m_Settings.Seed;

        const siv::PerlinNoise perlin{seed};
        
        constexpr float frequency = 1;
        constexpr float octaves = 5;
        constexpr float amplitude = 20;
        constexpr float groundOffset = 10;
        double fx = frequency / static_cast<double>(CHUNK_SIZE);
        double fy = frequency / static_cast<double>(CHUNK_SIZE);

        for (float z = 0; z < CHUNK_SIZE; z++)
        {
            for (float x = 0; x < CHUNK_SIZE; x++)
            {
                float h = perlin.octave2D_01((chunkPosition.x + fx * x), (chunkPosition.z + fy * z), octaves) *
                                  amplitude +
                        groundOffset;
                for (int y = 0; y < h; y++) { shapeData->SetSafe(glm::ivec3(x, y, z)); }
            }
        }
    }

    void TerrainGenerator::GenerateBlocks(const TerrainShape* shapeData, BlockData* data,
                                          glm::ivec3 chunkPosition) const
    {

        const siv::PerlinNoise::seed_type seed = m_Settings.Seed;
        const siv::PerlinNoise perlin{12345};

        constexpr float frequency = 8;
        constexpr float octaves = 8;
        constexpr float fx = frequency / static_cast<float>(CHUNK_SIZE);
        constexpr float fy = frequency / static_cast<float>(CHUNK_SIZE);

        for (uint32_t z = 0; z < CHUNK_SIZE; z++)
        {
            for (uint32_t x = 0; x < CHUNK_SIZE; x++)
            {
                uint32_t depth = 0;
                for (uint32_t y = CHUNK_SIZE - 1; y > 0; y--)
                {
                    if (shapeData->IsSolid(glm::ivec3(x, y, z)))
                    {
                        float set = perlin.octave2D_01(fx * x, fy * z, octaves);
                        if (set > 0.5 && depth < 1)
                        {
                            data->SetBlock(glm::ivec3(x, y, z), BlockType::GRASS);
                            depth++;
                        }
                        else { data->SetBlock(glm::ivec3(x, y, z), BlockType::STONE); }
                    }
                }
            }
        }
    }

    TerrainGenerationSettings TerrainGenerator::GetSettings() const { return m_Settings; }

    uint32_t TerrainGenerator::GetSeed() const { return m_Settings.Seed; }
}// namespace Engine
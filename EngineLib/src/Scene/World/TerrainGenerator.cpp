#include "TerrainGenerator.hpp"
#include <FastNoise/FastNoise.h>
#include <FastNoise/Metadata.h>
#include <Core/Timer.hpp>

namespace Engine
{

    void TerrainGenerator::Init(TerrainGenerationSettings settings) { m_Settings = settings; }

    void TerrainGenerator::Destroy() {}

    void TerrainGenerator::GenerateTerrainShape(TerrainShape* shapeData, glm::ivec3 chunkPosition) const
    {
        ScopedTimer timer("TerrainGenerator::GenerateTerrainShape");
        auto fnSimplex = FastNoise::New<FastNoise::OpenSimplex2>();
        auto fnFractal = FastNoise::New<FastNoise::FractalFBm>();
        constexpr float frequency = 0.02f;
        constexpr float octaves = 8;
        fnFractal->SetSource(fnSimplex);
        fnFractal->SetOctaveCount(octaves);
        fnFractal->SetLacunarity(1.58);
        fnFractal->SetWeightedStrength(0.4);
        fnFractal->SetGain(0.48);

        std::vector<float> heightMap(CHUNK_SIZE_SQUARE);
        fnFractal->GenUniformGrid2D(heightMap.data(), chunkPosition.x * CHUNK_SIZE, chunkPosition.z * CHUNK_SIZE,
                                    CHUNK_SIZE, CHUNK_SIZE, frequency, m_Settings.Seed);
        shapeData->Init(m_Settings.Seed);

        constexpr float amplitude = 12;
        constexpr float groundOffset = 10;

        for (float z = 0; z < CHUNK_SIZE; z++)
        {
            for (float x = 0; x < CHUNK_SIZE; x++)
            {
                float h = heightMap[z * CHUNK_SIZE + x] * amplitude + groundOffset;
                for (int y = 0; y < h; y++) { shapeData->SetSafe(glm::ivec3(x, y, z)); }
            }
        }
    }

    void TerrainGenerator::GenerateBlocks(const TerrainShape* shapeData, BlockData* data,
                                          glm::ivec3 chunkPosition) const
    {
        ScopedTimer timer("TerrainGenerator::GenerateBlocks");
        auto fnSimplex = FastNoise::New<FastNoise::OpenSimplex2>();
        auto fnFractal = FastNoise::New<FastNoise::FractalPingPong>();
        constexpr float frequency = 0.02;
        constexpr float octaves = 2;

        fnFractal->SetSource(fnSimplex);
        fnFractal->SetOctaveCount(octaves);
        fnFractal->SetLacunarity(1);
        fnFractal->SetWeightedStrength(1);
        // fnFractal->SetGain(0.48);
        fnFractal->SetPingPongStrength(1.0f);

        std::vector<float> heightMap(CHUNK_SIZE_CUBIC);
        fnFractal->GenUniformGrid3D(heightMap.data(), chunkPosition.x * CHUNK_SIZE, 0, chunkPosition.z * CHUNK_SIZE,
                                    CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, frequency, m_Settings.Seed);


        for (uint32_t z = 0; z < CHUNK_SIZE; z++)
        {
            for (uint32_t x = 0; x < CHUNK_SIZE; x++)
            {
                uint32_t depth = 0;
                for (uint32_t y = CHUNK_SIZE - 1; y > 0; y--)
                {
                    if (shapeData->IsSolid(glm::ivec3(x, y, z)))
                    {
                        float set = heightMap[ChunkPosition::GetIndex(glm::vec3{x, y, z})];
                        if (set >= 0.60 && depth < 1)
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

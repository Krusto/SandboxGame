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
        auto fnNoise = FastNoise::New<FastNoise::FractalRidged>();
        fnNoise->SetSource(fnSimplex);
        fnNoise->SetGain(0.54f);
        fnNoise->SetLacunarity(1.52f);
        fnNoise->SetWeightedStrength(2.58f);
        fnNoise->SetOctaveCount(4);

        constexpr float frequency = 0.002f;

        std::vector<float> continetalness(CHUNK_SIZE_SQUARE);
        fnNoise->GenUniformGrid2D(continetalness.data(), chunkPosition.x * CHUNK_SIZE, chunkPosition.z * CHUNK_SIZE,
                                  CHUNK_SIZE, CHUNK_SIZE, frequency, 1234);

        shapeData->Init(m_Settings.Seed);

        uint32_t chunkBaseY = chunkPosition.y * CHUNK_SIZE;
        LOG("%d\n", chunkBaseY);
        uint32_t chunkMaxY = chunkBaseY + CHUNK_SIZE;

        constexpr glm::vec2 splinePoint1(0.0f, 4);
        constexpr glm::vec2 splinePoint2(0.5f, 50);
        constexpr glm::vec2 splinePoint3(1.0f, 90);
        constexpr glm::vec2 splinePoint4(1.2f, 150);
        constexpr glm::vec2 splinePoint5(1.5f, 250);
        constexpr glm::vec2 splinePoint6(2.0f, 319);

        constexpr float slope1 = (splinePoint2.y - splinePoint1.y) / (splinePoint2.x - splinePoint1.x);
        constexpr float slope2 = (splinePoint3.y - splinePoint2.y) / (splinePoint3.x - splinePoint2.x);
        constexpr float slope3 = (splinePoint4.y - splinePoint3.y) / (splinePoint4.x - splinePoint3.x);
        constexpr float slope4 = (splinePoint5.y - splinePoint4.y) / (splinePoint5.x - splinePoint4.x);
        constexpr float slope5 = (splinePoint6.y - splinePoint5.y) / (splinePoint6.x - splinePoint5.x);

        for (float z = 0; z < CHUNK_SIZE; z++)
        {
            for (float x = 0; x < CHUNK_SIZE; x++)
            {
                float c = continetalness[z * CHUNK_SIZE + x] + 1;
                float height{};
                if (c >= splinePoint1.x && c < splinePoint2.x)
                {
                    height = slope1 * (c - splinePoint1.x) + splinePoint1.y;
                }
                else if (c >= splinePoint2.x && c < splinePoint3.x)
                {
                    height = slope2 * (c - splinePoint2.x) + splinePoint2.y;
                }
                else if (c >= splinePoint3.x && c < splinePoint4.x)
                {
                    height = slope3 * (c - splinePoint3.x) + splinePoint3.y;
                }
                else if (c >= splinePoint4.x && c < splinePoint5.x)
                {
                    height = slope4 * (c - splinePoint4.x) + splinePoint4.y;
                }
                else if (c >= splinePoint5.x && c < splinePoint6.x)
                {
                    height = slope5 * (c - splinePoint5.x) + splinePoint5.y;
                }
                height = height / 2;
                if (height > chunkBaseY && height <= chunkMaxY)
                {
                    uint32_t blockPosY = height - chunkBaseY;
                    for (uint32_t y = 0; y <= blockPosY; y++) { shapeData->Set(glm::ivec3{x, y, z}); }
                }
                else if (height > chunkMaxY)
                {
                    for (uint32_t y = 0; y < CHUNK_SIZE; y++) { shapeData->Set(glm::ivec3{x, y, z}); }
                }
            }
        }
    }

    void TerrainGenerator::GenerateBlocks(const TerrainShape* shapeData, BlockData* data,
                                          glm::ivec3 chunkPosition) const
    {
        auto fnSimplex = FastNoise::New<FastNoise::OpenSimplex2>();
        auto fnFractal = FastNoise::New<FastNoise::FractalPingPong>();
        constexpr float frequency = 0.02;
        constexpr float octaves = 2;

        fnFractal->SetSource(fnSimplex);
        fnFractal->SetOctaveCount(octaves);
        fnFractal->SetLacunarity(1);
        fnFractal->SetWeightedStrength(1);
        fnFractal->SetGain(0.48);
        fnFractal->SetPingPongStrength(1.0f);

        std::vector<float> heightMap(CHUNK_SIZE_CUBIC);
        fnFractal->GenUniformGrid3D(heightMap.data(), chunkPosition.x * CHUNK_SIZE, chunkPosition.y * CHUNK_SIZE,
                                    chunkPosition.z * CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, frequency,
                                    m_Settings.Seed);


        for (uint32_t z = 0; z < CHUNK_SIZE; z++)
        {
            for (uint32_t x = 0; x < CHUNK_SIZE; x++)
            {
                uint32_t depth = 0;
                for (int32_t y = CHUNK_SIZE - 1; y >= 0; y--)
                {
                    if (shapeData->IsSolid(glm::ivec3(x, y, z)))
                    {
                        //if (depth == 0) {
                        //float set = heightMap[ChunkPosition::GetIndex(glm::vec3{x, y, z})];
                        //if (set >= 0.40) { data->SetBlock(glm::ivec3(x, y, z), BlockType::SAND); }
                        //else { data->SetBlock(glm::ivec3(x, y, z), BlockType::GRASS); }
                        //}else
                        if (depth >= 0 && depth < 3) { data->SetBlock(glm::ivec3(x, y, z), BlockType::DIRT); }
                        else { data->SetBlock(glm::ivec3(x, y, z), BlockType::STONE); }

                        depth++;
                    }
                }
            }
        }
    }

    TerrainGenerationSettings TerrainGenerator::GetSettings() const { return m_Settings; }

    uint32_t TerrainGenerator::GetSeed() const { return m_Settings.Seed; }
}// namespace Engine

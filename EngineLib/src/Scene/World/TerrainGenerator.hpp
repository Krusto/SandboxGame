#include "ChunkConstraints.hpp"
#include "ChunkData.hpp"

struct TerrainGenerationSettings {
    uint32_t Seed;
};

class TerrainGenerator
{
public:
    TerrainGenerator() = default;
    ~TerrainGenerator() = default;

public:
    void Init(TerrainGenerationSettings settings);
    void Destroy();
    void GenerateTerrainShape(TerrainShapeData* shapeData, glm::ivec3 chunkPosition);

    TerrainGenerationSettings GetSettings() const;

    uint32_t GetSeed() const;

private:
    TerrainGenerationSettings m_Settings;
};

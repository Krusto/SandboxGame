#include "ChunkConstraints.hpp"
#include "BlockData.hpp"
#include "TerrainShape.hpp"

#include <filesystem>

namespace Engine
{

    struct TerrainGenerationSettings {
        uint32_t Seed{};
        std::filesystem::path AssetsDirectory;
        uint32_t minTerrainHeight = 10;
        uint32_t maxTerrainHeight = 150;
    };

    class TerrainGenerator
    {
    public:
        TerrainGenerator() = default;
        ~TerrainGenerator() = default;

    public:
        void Init(TerrainGenerationSettings settings);
        void Destroy();
        void GenerateTerrainShape(TerrainShape* shapeData, glm::ivec3 chunkPosition) const;
        void GenerateBlocks(const TerrainShape* shapeData, BlockData* data, glm::ivec3 chunkPosition) const;
        TerrainGenerationSettings GetSettings() const;

        uint32_t GetSeed() const;

    private:
        TerrainGenerationSettings m_Settings;
    };
}// namespace Engine
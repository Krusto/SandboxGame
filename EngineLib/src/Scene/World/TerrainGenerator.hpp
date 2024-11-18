#include "ChunkConstraints.hpp"
#include "BlockData.hpp"
#include "TerrainShape.hpp"

#include <filesystem>

namespace Engine
{

    struct TerrainGenerationSettings {
        uint32_t Seed{};
        std::filesystem::path AssetsDirectory;
        uint32_t GenerationDistance = 10;
        uint32_t minTerrainHeight = 10;
        uint32_t maxTerrainHeight = 150;
    };

    class TerrainGenerator
    {
    public:
        TerrainGenerator() = default;
        ~TerrainGenerator() = default;

    public:
        static void GenerateTerrainShape(TerrainGenerationSettings settings, TerrainShape* shapeData,
                                         glm::ivec3 chunkPosition);
        static void GenerateBlocks(TerrainGenerationSettings settings, const TerrainShape* shapeData, BlockData* data,
                                   glm::ivec3 chunkPosition);

    private:
    };
}// namespace Engine
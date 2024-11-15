#pragma once

#include "Chunk.hpp"
#include "BlockData.hpp"
#include "TerrainShape.hpp"
#include "ChunkMesh.hpp"
#include "TerrainGenerator.hpp"

namespace Engine
{

    class ChunkFactory
    {
    public:
        ChunkFactory() = default;
        ~ChunkFactory() = default;

    public:
        Chunk GenerateChunk(TerrainGenerationSettings* settings, glm::ivec3 chunkPosition) const;
        void DestroyChunk(Chunk chunk) const;

        static BlockData* GenerateBlockData(TerrainGenerationSettings* settings, TerrainShape* shapeData,
                                            glm::ivec3 chunkPosition);
        static void DestroyBlockData(BlockData* data);

        static TerrainShape* GenerateTerrainShape(TerrainGenerationSettings* settings, glm::ivec3 chunkPosition);
        static void DestroyTerrainShape(TerrainShape* data);

        static ChunkMesh* GenerateChunkMesh(BlockData* blockData);
        static void DestroyChunkMesh(ChunkMesh* mesh);


    private:
    };
}// namespace Engine

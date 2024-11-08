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
        void Init(TerrainGenerationSettings settings);

        Chunk GenerateChunk(glm::ivec3 chunkPosition) const;
        void DestroyChunk(Chunk chunk) const;

        BlockData* GenerateBlockData(TerrainShape* shapeData, glm::ivec3 chunkPosition) const;
        void DestroyBlockData(BlockData* data) const;
        TerrainShape* GenerateTerrainShape(glm::ivec3 chunkPosition) const;
        void DestroyTerrainShape(TerrainShape* data) const;
        ChunkMesh* GenerateChunkMesh(BlockData* blockData) const;
        void DestroyChunkMesh(ChunkMesh* mesh) const;

    private:
        TerrainGenerator m_Generator;
    };
}// namespace Engine

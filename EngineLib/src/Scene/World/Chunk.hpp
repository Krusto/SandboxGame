#pragma once
#include <Renderer/Renderer.hpp>

namespace Engine
{
    class TerrainShape;
    class BlockData;
    class ChunkMesh;

    struct Chunk {
        TerrainShape* terrainShape{};
        BlockData* blockData{};
        ChunkMesh* mesh{};
    };

}// namespace Engine
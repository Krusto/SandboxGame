#pragma once

#include "ChunkData.hpp"
#include "ChunkMesh.hpp"

class ChunkMeshFactory
{
public:
    static ChunkMesh* Generate(const TerrainShapeData* chunkData);

    static void Destroy(ChunkMesh* mesh);
};
#include "ChunkMeshFactory.hpp"
#include "ChunkData.hpp"
#include "ChunkMesh.hpp"

ChunkMesh* ChunkMeshFactory::Generate(const TerrainShapeData* chunkData)
{
    auto ptr = new ChunkMesh();
    ptr->Generate(chunkData);
    return ptr;
}

void ChunkMeshFactory::Destroy(ChunkMesh* mesh)
{
    mesh->Destroy();
    delete mesh;
}
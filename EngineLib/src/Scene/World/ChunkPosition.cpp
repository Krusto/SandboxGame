#include "ChunkPosition.hpp"

size_t ChunkPosition::GetIndex(glm::vec3 position) { return GetIndex((glm::ivec3) position); }

size_t ChunkPosition::GetIndex(glm::ivec3 position)
{
    return position.x + position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARE;
}

size_t ChunkPosition::GetIndexX_P(glm::vec3 position)
{
    return (size_t) ((uint64_t) position.z + ((uint64_t) position.y * CHUNK_SIZE_SQUARE_PADDED) +
                     CHUNK_SIZE_SQUARE_PADDED);
}

size_t ChunkPosition::GetIndexX_P(glm::ivec3 position)
{
    return (size_t) ((uint64_t) position.x + ((uint64_t) position.z * CHUNK_SIZE_SQUARE_PADDED) +
                     CHUNK_SIZE_SQUARE_PADDED);
}

size_t ChunkPosition::GetIndexY_P(glm::vec3 position)
{
    return (size_t) ((uint64_t) position.x + ((uint64_t) position.z * CHUNK_SIZE_SQUARE_PADDED));
}

size_t ChunkPosition::GetIndexY_P(glm::ivec3 position)
{
    return (size_t) ((uint64_t) position.x + ((uint64_t) position.z * CHUNK_SIZE_SQUARE_PADDED));
}

size_t ChunkPosition::GetIndexZ_P(glm::vec3 position)
{
    return (size_t) ((uint64_t) position.x + ((uint64_t) position.y * CHUNK_SIZE_SQUARE_PADDED) +
                     CHUNK_SIZE_SQUARE_PADDED * 2);
}

size_t ChunkPosition::GetIndexZ_P(glm::ivec3 position)
{
    return (size_t) ((uint64_t) position.x + ((uint64_t) position.y * CHUNK_SIZE_SQUARE_PADDED) +
                     CHUNK_SIZE_SQUARE_PADDED * 2);
}

size_t ChunkPosition::GetRowIndex(glm::vec3 position)
{
    return position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARE;
}

size_t ChunkPosition::GetRowIndex(glm::ivec3 position)
{
    return position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARE;
}

glm::vec3 ChunkPosition::GetPosition(size_t index)
{
    return glm::vec3{index % CHUNK_SIZE, (index / CHUNK_SIZE) % CHUNK_SIZE, index / CHUNK_SIZE_SQUARE};
}

glm::ivec3 ChunkPosition::GetPosition(int32_t index)
{
    return glm::ivec3{index % CHUNK_SIZE, (index / CHUNK_SIZE) % CHUNK_SIZE, index / CHUNK_SIZE_SQUARE};
}

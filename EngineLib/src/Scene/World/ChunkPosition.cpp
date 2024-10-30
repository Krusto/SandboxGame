#include "ChunkPosition.hpp"

constexpr size_t ChunkPosition::GetIndex(glm::vec3 position)
{
    return position.x + position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARE;
}

constexpr size_t ChunkPosition::GetIndex(glm::ivec3 position)
{
    return position.x + position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARE;
}

constexpr size_t ChunkPosition::GetRowIndex(glm::vec3 position)
{
    return position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARE;
}

constexpr size_t ChunkPosition::GetRowIndex(glm::ivec3 position)
{
    return position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARE;
}

constexpr glm::vec3 ChunkPosition::GetPosition(size_t index)
{
    return glm::vec3{index % CHUNK_SIZE, (index / CHUNK_SIZE) % CHUNK_SIZE, index / CHUNK_SIZE_SQUARE};
}

constexpr glm::ivec3 ChunkPosition::GetPosition(int32_t index)
{
    return glm::ivec3{index % CHUNK_SIZE, (index / CHUNK_SIZE) % CHUNK_SIZE, index / CHUNK_SIZE_SQUARE};
}

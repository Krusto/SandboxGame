#pragma once
#include <cstdint>
#include <glm/glm.hpp>

constexpr int32_t CHUNK_SIZE = 32;
constexpr int32_t CHUNK_SIZE_SQUARE = CHUNK_SIZE * CHUNK_SIZE;
constexpr int32_t CHUNK_SIZE_CUBIC = CHUNK_SIZE_SQUARE * CHUNK_SIZE;

constexpr size_t GetIndex(glm::vec3 position)
{
    return position.x + position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARE;
}

constexpr size_t GetIndex(glm::ivec3 position)
{
    return position.x + position.y * CHUNK_SIZE + position.z * CHUNK_SIZE_SQUARE;
}

constexpr glm::vec3 GetPosition(size_t index)
{
    return glm::vec3{index % CHUNK_SIZE, (index / CHUNK_SIZE) % CHUNK_SIZE, index / CHUNK_SIZE_SQUARE};
}

constexpr glm::ivec3 GetPosition(int32_t index)
{
    return glm::ivec3{index % CHUNK_SIZE, (index / CHUNK_SIZE) % CHUNK_SIZE, index / CHUNK_SIZE_SQUARE};
}
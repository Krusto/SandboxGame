#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include "ChunkConstraints.hpp"

class ChunkPosition
{
public:
    constexpr static size_t GetIndex(glm::vec3 position);

    constexpr static size_t GetIndex(glm::ivec3 position);

    constexpr static size_t GetRowIndex(glm::vec3 position);
    constexpr static size_t GetRowIndex(glm::ivec3 position);

    constexpr static glm::vec3 GetPosition(size_t index);

    constexpr static glm::ivec3 GetPosition(int32_t index);
};
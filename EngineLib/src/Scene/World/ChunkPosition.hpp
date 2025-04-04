#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include "ChunkConstraints.hpp"

namespace Engine
{
    class ChunkPosition
    {
    public:
        static size_t GetIndex(glm::vec3 position);
        static size_t GetIndex(glm::ivec3 position);

        static size_t GetIndexP(glm::vec3 position);
        static size_t GetIndexP(glm::ivec3 position);

        static size_t GetRowIndex(glm::ivec3 position);
        static size_t GetRowIndex(glm::vec3 position);

        static size_t GetIndexX_P(glm::vec3 position);
        static size_t GetIndexX_P(glm::ivec3 position);

        static size_t GetIndexY_P(glm::vec3 position);
        static size_t GetIndexY_P(glm::ivec3 position);

        static size_t GetIndexZ_P(glm::vec3 position);
        static size_t GetIndexZ_P(glm::ivec3 position);


        static glm::vec3 GetPosition(size_t index);

        static glm::ivec3 GetPosition(int32_t index);
    };
}// namespace Engine
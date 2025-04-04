#pragma once
#include <Core/Types.hpp>
#include <glm/glm.hpp>
#include "ChunkConstraints.hpp"

namespace Engine
{
    class ChunkPosition
    {
    public:
        staticstd::size_t GetIndex(glm::vec3 position);
        staticstd::size_t GetIndex(glm::ivec3 position);

        staticstd::size_t GetIndexP(glm::vec3 position);
        staticstd::size_t GetIndexP(glm::ivec3 position);

        staticstd::size_t GetRowIndex(glm::ivec3 position);
        staticstd::size_t GetRowIndex(glm::vec3 position);

        staticstd::size_t GetIndexX_P(glm::vec3 position);
        staticstd::size_t GetIndexX_P(glm::ivec3 position);

        staticstd::size_t GetIndexY_P(glm::vec3 position);
        staticstd::size_t GetIndexY_P(glm::ivec3 position);

        staticstd::size_t GetIndexZ_P(glm::vec3 position);
        staticstd::size_t GetIndexZ_P(glm::ivec3 position);


        static glm::vec3 GetPosition(size_t index);

        static glm::ivec3 GetPosition(int32_t index);
    };
}// namespace Engine
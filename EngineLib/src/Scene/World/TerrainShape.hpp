#pragma once
#include <glm/glm.hpp>
#include "ChunkPosition.hpp"
#include "BlockRegistry.hpp"

namespace Engine
{


    class TerrainShape
    {
    public:
        TerrainShape() = default;
        ~TerrainShape();

    public:
        void Init(uint32_t seed);
        void Init(const TerrainShape& other);
        void Destroy();

        bool IsSolid(glm::ivec3 position) const;
        bool IsSolid(glm::vec3 position) const;
        bool IsSolid(size_t index) const;

        bool IsSolidSafe(glm::ivec3 position) const;
        bool IsSolidSafe(glm::vec3 position) const;
        bool IsSolidSafe(size_t index) const;

        void Set();
        void Set(glm::vec3 position);
        void Set(glm::ivec3 position);

        void SetSafe(glm::vec3 position);
        void SetSafe(glm::ivec3 position);

        void Unset();
        void Unset(glm::vec3 position);
        void Unset(glm::ivec3 position);

    private:
        uint32_t* m_Data{};
    };
}// namespace Engine
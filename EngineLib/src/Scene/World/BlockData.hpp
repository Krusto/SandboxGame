#pragma once
#include <glm/glm.hpp>
#include "ChunkPosition.hpp"

namespace Engine
{
    class BlockData
    {
    public:
        BlockData() = default;
        ~BlockData() = default;

    public:
        void Init(uint32_t seed);
        void Destroy();

        uint8_t GetBlock(glm::ivec3 position) const;
        uint8_t GetBlock(glm::vec3 position) const;
        uint8_t GetBlock(size_t index) const;

        void SetBlock(glm::ivec3 position, uint8_t block);
        void SetBlock(glm::vec3 position, uint8_t block);
        void SetBlock(size_t index, uint8_t block);

        bool IsSolid(glm::ivec3 position) const;
        bool IsSolid(glm::vec3 position) const;
        bool IsSolid(size_t index) const;

        bool IsSolidSafe(glm::ivec3 position) const;
        bool IsSolidSafe(glm::vec3 position) const;
        bool IsSolidSafe(size_t index) const;

        uint8_t* RawData();

        const uint8_t* RawData() const;

        conststd::size_t Size() const;

    private:
        uint8_t* m_Data;
    };
}// namespace Engine
#include <Core/Log.hpp>
#include <Core/Allocator.hpp>
#include <algorithm>

#include "BlockRegistry.hpp"
#include "BlockData.hpp"

namespace Engine
{
    void BlockData::Init(uint32_t seed)
    {
        AllocateArray(uint8_t, m_Data, CHUNK_SIZE_CUBIC);
        for (size_t i = 0; i < CHUNK_SIZE_CUBIC; i++) { m_Data[i] = 0; }
    }

    void BlockData::Destroy()
    {
        DeallocateArray(m_Data);
        m_Data = nullptr;
    }

    bool BlockData::IsSolid(glm::ivec3 position) const { return IsSolid(ChunkPosition::GetIndex(position)); }

    bool BlockData::IsSolid(glm::vec3 position) const { return IsSolid((glm::ivec3) position); }

    bool BlockData::IsSolid(size_t index) const { return m_Data[index]; }

    bool BlockData::IsSolidSafe(glm::ivec3 position) const
    {
        if (position.x < 0 || position.y < 0 || position.z < 0) { return false; }
        if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return false; }
        return IsSolid(position);
    }

    bool BlockData::IsSolidSafe(glm::vec3 position) const
    {
        if (position.x < 0 || position.y < 0 || position.z < 0) { return false; }
        if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return false; }
        return IsSolid(position);
    }

    bool BlockData::IsSolidSafe(size_t index) const
    {
        if (index < 0 || index >= CHUNK_SIZE_CUBIC) { return false; }
        return IsSolid(index);
    }

    uint8_t BlockData::GetBlock(glm::ivec3 position) const { return m_Data[ChunkPosition::GetIndex(position)]; }

    uint8_t BlockData::GetBlock(glm::vec3 position) const { return m_Data[ChunkPosition::GetIndex(position)]; }

    uint8_t BlockData::GetBlock(size_t index) const { return m_Data[index]; }

    void BlockData::SetBlock(glm::ivec3 position, uint8_t block) { m_Data[ChunkPosition::GetIndex(position)] = block; }

    void BlockData::SetBlock(glm::vec3 position, uint8_t block) { m_Data[ChunkPosition::GetIndex(position)] = block; }

    void BlockData::SetBlock(size_t index, uint8_t block) { m_Data[index] = block; }

    uint8_t* BlockData::RawData() { return m_Data; }

    const uint8_t* BlockData::RawData() const { return m_Data; }

    const size_t BlockData::Size() const { return CHUNK_SIZE_CUBIC; }
}// namespace Engine
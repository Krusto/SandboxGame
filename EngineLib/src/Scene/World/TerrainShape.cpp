#include "TerrainShape.hpp"
#include <Core/Log.hpp>
#include <Core/Allocator.hpp>
#include <algorithm>

namespace Engine
{

    TerrainShape::~TerrainShape() { Destroy(); }

    void TerrainShape::Init(uint32_t seed)
    {
        m_Data = Engine::Allocator::AllocateArray<uint32_t>(CHUNK_SIZE_SQUARE);

        std::fill_n(m_Data, CHUNK_SIZE_SQUARE, 0);
    }

    void TerrainShape::Init(const TerrainShape& other)
    {


        m_Data = Engine::Allocator::AllocateArray<uint32_t>(CHUNK_SIZE_SQUARE);
        std::copy(other.m_Data, other.m_Data + CHUNK_SIZE_SQUARE, m_Data);
    }

    void TerrainShape::Destroy()
    {
        if (m_Data)
        {
            Engine::Allocator::DeallocateArray(m_Data);
            m_Data = nullptr;
        }
    }

    bool TerrainShape::IsSolid(glm::ivec3 position) const
    {
        size_t rowIndex = ChunkPosition::GetRowIndex(position);
        return (m_Data[rowIndex] >> (size_t) position.x) & 1;
    }

    bool TerrainShape::IsSolid(glm::vec3 position) const
    {
        size_t rowIndex = ChunkPosition::GetRowIndex(position);
        return (m_Data[rowIndex] >> (size_t) position.x) & 1;
    }

    bool TerrainShape::IsSolid(size_t index) const { return m_Data[index]; }

    bool TerrainShape::IsSolidSafe(glm::ivec3 position) const
    {
        if (position.x < 0 || position.y < 0 || position.z < 0) { return false; }
        if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return false; }
        return IsSolid(position);
    }

    bool TerrainShape::IsSolidSafe(glm::vec3 position) const
    {
        if (position.x < 0 || position.y < 0 || position.z < 0) { return false; }
        if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return false; }
        return IsSolid(position);
    }

    bool TerrainShape::IsSolidSafe(size_t index) const
    {
        if (index < 0 || index >= CHUNK_SIZE_SQUARE_COMPRESSED) { return false; }
        return IsSolid(index);
    }

    void TerrainShape::Set() { std::fill_n(m_Data, CHUNK_SIZE_SQUARE_COMPRESSED, true); }

    void TerrainShape::Set(glm::vec3 position)
    {
        size_t rowIndex = ChunkPosition::GetRowIndex(position);
        m_Data[rowIndex] |= 1 << (size_t) position.x;
    }

    void TerrainShape::SetSafe(glm::vec3 position)
    {
        if (position.x < 0 || position.y < 0 || position.z < 0) { return; }
        if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return; }
        size_t rowIndex = ChunkPosition::GetRowIndex(position);
        m_Data[rowIndex] |= 1 << (size_t) position.x;
    }

    void TerrainShape::SetSafe(glm::ivec3 position)
    {
        if (position.x < 0 || position.y < 0 || position.z < 0) { return; }
        if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return; }

        size_t rowIndex = ChunkPosition::GetRowIndex(position);
        m_Data[rowIndex] |= 1 << (size_t) position.x;
    }

    void TerrainShape::Unset(glm::vec3 position)
    {
        size_t rowIndex = ChunkPosition::GetRowIndex(position);
        m_Data[rowIndex] ^= 1 << (size_t) position.x;
        m_Data[ChunkPosition::GetIndex(position)] = false;
    }

    void TerrainShape::Set(glm::ivec3 position) { Set((glm::vec3) position); }

    void TerrainShape::Unset() { std::fill_n(m_Data, CHUNK_SIZE_SQUARE_COMPRESSED, 0); }

    void TerrainShape::Unset(glm::ivec3 position) { Unset((glm::vec3) position); }

}// namespace Engine
#include "ChunkData.hpp"
#include <Core/Log.hpp>
#include <Core/Allocator.hpp>
#include <algorithm>

TerrainShapeData::~TerrainShapeData() { Destroy(); }

void TerrainShapeData::Init(uint32_t seed)
{
    m_Data = Engine::Allocator::AllocateArray<uint8_t>(CHUNK_SIZE_CUBIC);
    std::for_each(m_Data, m_Data + CHUNK_SIZE_CUBIC, [](uint8_t& value) { value = 0; });
}

void TerrainShapeData::Init(const TerrainShapeData& other)
{
    m_Data = Engine::Allocator::AllocateArray<uint8_t>(CHUNK_SIZE_CUBIC);
    std::copy(other.m_Data, other.m_Data + CHUNK_SIZE_CUBIC, m_Data);
}

void TerrainShapeData::Destroy()
{
    if (m_Data)
    {
        Engine::Allocator::DeallocateArray(m_Data);
        m_Data = nullptr;
    }
}

bool TerrainShapeData::IsSolid(glm::ivec3 position) const
{
    // size_t index = ChunkPosition::GetIndex(position);
    // size_t byteIndex = index / BlockPerByte;
    // size_t bitIndex = index % BlockPerByte;

    // uint64_t value = (m_Data[byteIndex] >> bitIndex) & 0x1;
    // return value == 1;

    return IsSolid(ChunkPosition::GetIndex(position));
}

bool TerrainShapeData::IsSolid(glm::vec3 position) const { return IsSolid((glm::ivec3) position); }

bool TerrainShapeData::IsSolid(size_t index) const
{
    // size_t byteIndex = index / BlockPerByte;
    // size_t bitIndex = index % BlockPerByte;

    // uint64_t value = (m_Data[byteIndex] >> bitIndex) & 0x1;
    // return value == 1;
    return m_Data[index];
}

bool TerrainShapeData::IsSolidSafe(glm::ivec3 position) const
{
    if (position.x < 0 || position.y < 0 || position.z < 0) { return false; }
    if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return false; }
    return IsSolid(position);
}

bool TerrainShapeData::IsSolidSafe(glm::vec3 position) const
{
    if (position.x < 0 || position.y < 0 || position.z < 0) { return false; }
    if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return false; }
    return IsSolid(position);
}

bool TerrainShapeData::IsSolidSafe(size_t index) const
{
    if (index < 0 || index >= CHUNK_SIZE_CUBIC) { return false; }
    return IsSolid(index);
}

void TerrainShapeData::Set()
{
    std::for_each(m_Data, m_Data + CHUNK_SIZE_CUBIC, [](uint8_t& value) { value = UINT64_MAX; });
}

void TerrainShapeData::Set(glm::vec3 position) { m_Data[ChunkPosition::GetIndex(position)] = true; }

void TerrainShapeData::Unset(glm::vec3 position) { m_Data[ChunkPosition::GetIndex(position)] = false; }

void TerrainShapeData::Set(glm::ivec3 position) { Set((glm::vec3) position); }

void TerrainShapeData::Unset()
{
    std::for_each(m_Data, m_Data + CHUNK_SIZE_CUBIC, [](uint8_t& value) { value = 0; });
}

void TerrainShapeData::Unset(glm::ivec3 position) { Unset((glm::vec3) position); }

uint64_t TerrainShapeData::GetRow(uint32_t y, uint32_t z) const
{
    return m_Data[y * CHUNK_SIZE + z * CHUNK_SIZE_SQUARE];
}

void ChunkData::Init(uint32_t seed)
{
    m_Data = Engine::Allocator::AllocateArray<uint8_t>(CHUNK_SIZE_CUBIC);
    for (size_t i = 0; i < CHUNK_SIZE_CUBIC; i++) { m_Data[i] = 0; }
}

void ChunkData::Destroy()
{
    Engine::Allocator::DeallocateArray(m_Data);
    m_Data = nullptr;
}

uint8_t ChunkData::GetBlock(glm::ivec3 position) const { return m_Data[ChunkPosition::GetIndex(position)]; }

uint8_t ChunkData::GetBlock(glm::vec3 position) const { return m_Data[ChunkPosition::GetIndex(position)]; }

uint8_t ChunkData::GetBlock(size_t index) const { return m_Data[index]; }

uint8_t* ChunkData::RawData() { return m_Data; }

const uint8_t* ChunkData::RawData() const { return m_Data; }

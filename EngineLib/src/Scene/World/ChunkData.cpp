#include "ChunkData.hpp"

void TerrainShapeData::Init(uint32_t seed)
{
    m_Data = new uint64_t[TerrainShapeCompresssedSize];
    for (size_t i = 0; i < TerrainShapeCompresssedSize; i++) { m_Data[i] = UINT64_MAX; }
}

void TerrainShapeData::Destroy() { delete[] m_Data; }

bool TerrainShapeData::IsSolid(glm::ivec3 position) const { return GetBit(ChunkPosition::GetIndex(position)); }

bool TerrainShapeData::IsSolid(glm::vec3 position) const { return GetBit(ChunkPosition::GetIndex(position)); }

bool TerrainShapeData::IsSolid(size_t index) const { return GetBit(index); }

bool TerrainShapeData::IsSolidSafe(glm::ivec3 position) const
{
    if (position.x < 0 || position.y < 0 || position.z < 0) { return false; }
    if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return false; }
    return GetBit(ChunkPosition::GetIndex(position));
}

bool TerrainShapeData::IsSolidSafe(glm::vec3 position) const
{
    if (position.x < 0 || position.y < 0 || position.z < 0) { return false; }
    if (position.x >= CHUNK_SIZE || position.y >= CHUNK_SIZE || position.z >= CHUNK_SIZE) { return false; }
    return GetBit(ChunkPosition::GetIndex(position));
}

bool TerrainShapeData::IsSolidSafe(size_t index) const
{
    if (index < 0 || index >= CHUNK_SIZE_CUBIC) { return false; }
    else { return GetBit(index); }
}

bool TerrainShapeData::GetBit(size_t index) const
{
    size_t byteIndex = index / BlockPerByte;
    size_t bitIndex = index % BlockPerByte;
    uint64_t value = m_Data[byteIndex];
    return (value & (1 << bitIndex)) > 0;
}

void TerrainShapeData::SetBit(size_t index, bool value)
{
    size_t byteIndex = index / BlockPerByte;
    size_t bitIndex = index % BlockPerByte;
    if (value) { m_Data[byteIndex] |= (1 << bitIndex); }
    else { m_Data[byteIndex] &= ~(1 << bitIndex); }
}

void ChunkData::Init(uint32_t seed)
{
    m_Data = new uint8_t[CHUNK_SIZE_CUBIC];
    for (size_t i = 0; i < CHUNK_SIZE_CUBIC; i++) { m_Data[i] = 0; }
}

void ChunkData::Destroy() { delete[] m_Data; }

uint8_t ChunkData::GetBlock(glm::ivec3 position) const { return m_Data[ChunkPosition::GetIndex(position)]; }

uint8_t ChunkData::GetBlock(glm::vec3 position) const { return m_Data[ChunkPosition::GetIndex(position)]; }

uint8_t ChunkData::GetBlock(size_t index) const { return m_Data[index]; }

uint8_t* ChunkData::RawData() { return m_Data; }

const uint8_t* ChunkData::RawData() const { return m_Data; }

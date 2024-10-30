#pragma once
#include <glm/glm.hpp>
#include "ChunkPosition.hpp"
#include "BlockRegistry.hpp"

constexpr size_t BlockPerByte = 8;
constexpr size_t TerrainShapeCompresssedSize = CHUNK_SIZE_CUBIC / BlockPerByte;

class TerrainShapeData
{
public:
    TerrainShapeData() = default;
    ~TerrainShapeData() = default;

public:
    void Init(uint32_t seed);
    void Destroy();

    bool IsSolid(glm::ivec3 position) const;
    bool IsSolid(glm::vec3 position) const;
    bool IsSolid(size_t index) const;

    bool IsSolidSafe(glm::ivec3 position) const;
    bool IsSolidSafe(glm::vec3 position) const;
    bool IsSolidSafe(size_t index) const;

    uint64_t GetRow(size_t index) const { return m_Data[index]; }
    uint64_t GetRow(glm::vec3 position) const { return GetRow(ChunkPosition::GetIndex(position)); }
    uint64_t GetRow(glm::ivec3 position) const { return GetRow(ChunkPosition::GetIndex(position)); }

private:
    bool GetBit(size_t index) const;
    void SetBit(size_t index, bool value);

    uint64_t* m_Data;
};

class ChunkData
{
public:
    ChunkData() = default;
    ~ChunkData() = default;

public:
    void Init(uint32_t seed);
    void Destroy();

    uint8_t GetBlock(glm::ivec3 position) const;
    uint8_t GetBlock(glm::vec3 position) const;
    uint8_t GetBlock(size_t index) const;


    uint8_t* RawData();

    const uint8_t* RawData() const;

private:
    uint8_t* m_Data;
};

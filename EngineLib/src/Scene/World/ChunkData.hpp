#pragma once
#include <glm/glm.hpp>
#include "ChunkPosition.hpp"
#include "BlockRegistry.hpp"

constexpr size_t BlockPerByte = 8;
constexpr size_t TerrainShapeCompresssedSize = CHUNK_SIZE_CUBIC;

class TerrainShapeData
{
public:
    TerrainShapeData() = default;
    ~TerrainShapeData();

public:
    void Init(uint32_t seed);
    void Init(const TerrainShapeData& other);
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
    void Unset();
    void Unset(glm::vec3 position);
    void Unset(glm::ivec3 position);

    uint64_t GetRow(uint32_t y, uint32_t z) const;

    uint8_t* m_Data;

private:
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

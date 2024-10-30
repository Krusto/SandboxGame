#pragma once
#include <array>
#include <string>
#include <string_view>

class BlockRegistry
{
public:
    BlockRegistry() = default;
    ~BlockRegistry() = default;

public:
    static void Init();
    static void Destroy();

    static std::string GetBlockName(uint32_t id);
    static uint32_t GetBlockID(std::string_view name);

public:
    enum BlockType
    {
        AIR = 0,
        STONE = 1,
        GRASS = 2,
        DIRT = 3,
        COBBLESTONE = 4,
        WOOD = 5,
        SAPLING = 6,
        BEDROCK = 7,
        WATER = 8
    };

private:
    static std::array<std::string, 256> s_BlockNames;
};

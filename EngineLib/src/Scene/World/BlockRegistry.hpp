#pragma once
#include <array>
#include <string>
#include <string_view>
#include <unordered_map>

namespace BlockType
{
    inline constexpr uint32_t AIR = 0;
    inline constexpr uint32_t STONE = 1;
    inline constexpr uint32_t GRASS = 2;
    inline constexpr uint32_t DIRT = 3;
    inline constexpr uint32_t COBBLESTONE = 4;
    inline constexpr uint32_t WOOD = 5;
    inline constexpr uint32_t SAPLING = 6;
    inline constexpr uint32_t BEDROCK = 7;
    inline constexpr uint32_t WATER = 8;
    inline constexpr uint32_t MAX = 9;
}// namespace BlockType

struct BlockInfo {

    uint32_t Type;
    std::string_view Name;
    std::string_view Texture;
};

class BlockRegistry
{
public:
    BlockRegistry() = default;
    ~BlockRegistry() = default;

public:
    static void Init();
    static void Destroy();

    static const std::string_view GetBlockName(uint32_t id);
    static uint32_t GetBlockID(std::string_view name);

private:
    static std::array<BlockInfo, 256> s_BlockInfos;
    static std::unordered_map<std::string, uint32_t> s_BlockIDs;
};

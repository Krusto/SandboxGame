#include "BlockRegistry.hpp"
#include <algorithm>

namespace Engine
{
    std::array<BlockInfo, 256> BlockRegistry::s_BlockInfos{};
    std::unordered_map<std::string, uint32_t> BlockRegistry::s_BlockIDs{};

    inline constexpr BlockInfo Air_Block = BlockInfo{BlockType::AIR, "Air", {.numTextures = 1, .data = {"Air"}}};
    inline constexpr BlockInfo Stone_Block =
            BlockInfo{BlockType::STONE, "Stone", {.numTextures = 1, .data = {"Stone.png"}}};
    inline constexpr BlockInfo Grass_Block =
            BlockInfo{BlockType::GRASS, "Grass", {.numTextures = 1, .data = {"Grass.png"}}};
    inline constexpr BlockInfo Dirt_Block =
            BlockInfo{BlockType::DIRT, "Dirt", {.numTextures = 1, .data = {"Dirt.png"}}};
    inline constexpr BlockInfo Cobblestone_Block =
            BlockInfo{BlockType::COBBLESTONE, "Cobblestone", {.numTextures = 1, .data = {"Cobblestone.png"}}};
    inline constexpr BlockInfo Wood_Block =
            BlockInfo{BlockType::WOOD, "Wood", {.numTextures = 1, .data = {"Wood.png"}}};
    inline constexpr BlockInfo Sapling_Block =
            BlockInfo{BlockType::SAPLING, "Sapling", {.numTextures = 1, .data = {"Sapling.png"}}};
    inline constexpr BlockInfo Bedrock_Block =
            BlockInfo{BlockType::BEDROCK, "Bedrock", {.numTextures = 1, .data = {"Bedrock.png"}}};
    inline constexpr BlockInfo Water_Block =
            BlockInfo{BlockType::WATER, "Water", {.numTextures = 1, .data = {"Water.png"}}};

    void BlockRegistry::Init()
    {
        s_BlockInfos[BlockType::AIR] = Air_Block;
        s_BlockInfos[BlockType::STONE] = Stone_Block;
        s_BlockInfos[BlockType::GRASS] = Grass_Block;
        s_BlockInfos[BlockType::DIRT] = Dirt_Block;
        s_BlockInfos[BlockType::COBBLESTONE] = Cobblestone_Block;
        s_BlockInfos[BlockType::WOOD] = Wood_Block;
        s_BlockInfos[BlockType::SAPLING] = Sapling_Block;
        s_BlockInfos[BlockType::BEDROCK] = Bedrock_Block;
        s_BlockInfos[BlockType::WATER] = Water_Block;
        for (int i = BlockType::MAX; i < 256; i++) { s_BlockInfos[i].Name = "Null"; }

        for (uint32_t i = 0; i < BlockType::MAX; i++) { s_BlockIDs[std::string(s_BlockInfos[i].Name)] = i; }
    }

    void BlockRegistry::Destroy() {}

    const std::string_view BlockRegistry::GetBlockName(uint32_t id) { return s_BlockInfos[id].Name; }

    uint32_t BlockRegistry::GetBlockID(std::string_view name)
    {
        if (s_BlockIDs.find(std::string(name)) != s_BlockIDs.end()) { return s_BlockIDs[std::string(name)]; }
        return 0;
    }

    std::unordered_map<uint32_t, std::string> BlockRegistry::AssignTexturesToIndices()
    {
        std::unordered_map<uint32_t, std::string> textureIndices;
        for (auto& blockInfo: s_BlockInfos)
        {
            if (blockInfo.Name != "Null" && blockInfo.Name != "Air" && blockInfo.Textures.numTextures > 0)
            {
                for (uint32_t textureIndex = 0; textureIndex < blockInfo.Textures.numTextures; textureIndex++)
                {
                    if (blockInfo.Textures.numTextures == 1)
                    {
                        textureIndices[blockInfo.Type - 1] = blockInfo.Textures.data[0];
                        break;
                    }
                }
            }
        }
        return textureIndices;
    }
}// namespace Engine
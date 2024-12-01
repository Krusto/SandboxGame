#include "BlockRegistry.hpp"
#include <algorithm>

namespace Engine
{
    std::array<BlockInfo, 256> BlockRegistry::s_BlockInfos{};
    std::unordered_map<std::string, uint32_t> BlockRegistry::s_BlockIDs{};
    std::unordered_map<size_t, const char*> BlockRegistry::s_Textures{
            {0, "Air"},           {1, "greystone.png"},  {2, "dirt.png"},
            {3, "grass_top.png"}, {4, "dirt_grass.png"}, {5, "gravel_stone.png"},
            {6, "wood.png"},      {7, "grass1.png"},     {8, "greysand.png"},
            {9, "water.png"},     {10, "sand.png"}};

    inline constexpr BlockInfo Air_Block =
            BlockInfo{BlockType::AIR, "Air", {.numTextures = 1, .flags = TextureType::TOP, .textureIDs = {0}}};
    inline constexpr BlockInfo Stone_Block =
            BlockInfo{BlockType::STONE, "Stone", {.numTextures = 1, .flags = TextureType::TOP, .textureIDs = {1}}};
    inline constexpr BlockInfo Dirt_Block =
            BlockInfo{BlockType::DIRT, "Dirt", {.numTextures = 1, .flags = TextureType::TOP, .textureIDs = {2}}};
    inline constexpr BlockInfo Grass_Block =
            BlockInfo{BlockType::GRASS,
                      "Grass",
                      {.numTextures = 2, .flags = TextureType::TOP | TextureType::SIDES, .textureIDs = {3, 4}}};
    inline constexpr BlockInfo Cobblestone_Block =
            BlockInfo{BlockType::COBBLESTONE,
                      "Cobblestone",
                      {.numTextures = 1, .flags = TextureType::TOP, .textureIDs = {5}}};
    inline constexpr BlockInfo Wood_Block =
            BlockInfo{BlockType::WOOD, "Wood", {.numTextures = 1, .flags = TextureType::TOP, .textureIDs = {6}}};
    inline constexpr BlockInfo Sapling_Block =
            BlockInfo{BlockType::SAPLING, "Sapling", {.numTextures = 1, .flags = TextureType::TOP, .textureIDs = {7}}};
    inline constexpr BlockInfo Bedrock_Block =
            BlockInfo{BlockType::BEDROCK, "Bedrock", {.numTextures = 1, .flags = TextureType::TOP, .textureIDs = {8}}};
    inline constexpr BlockInfo Water_Block =
            BlockInfo{BlockType::WATER, "Water", {.numTextures = 1, .flags = TextureType::TOP, .textureIDs = {9}}};
    inline constexpr BlockInfo Sand_Block =
            BlockInfo{BlockType::SAND, "Sand", {.numTextures = 1, .flags = TextureType::TOP, .textureIDs = {10}}};

    void BlockRegistry::Init()
    {
        s_BlockInfos[BlockType::AIR] = Air_Block;
        s_BlockInfos[BlockType::STONE] = Stone_Block;
        s_BlockInfos[BlockType::DIRT] = Dirt_Block;
        s_BlockInfos[BlockType::GRASS] = Grass_Block;
        s_BlockInfos[BlockType::COBBLESTONE] = Cobblestone_Block;
        s_BlockInfos[BlockType::WOOD] = Wood_Block;
        s_BlockInfos[BlockType::SAPLING] = Sapling_Block;
        s_BlockInfos[BlockType::BEDROCK] = Bedrock_Block;
        s_BlockInfos[BlockType::WATER] = Water_Block;
        s_BlockInfos[BlockType::SAND] = Sand_Block;
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

        size_t currentIndex = 0;
        for (auto& blockInfo: s_BlockInfos)
        {
            if (blockInfo.Name != "Null" && blockInfo.Name != "Air" && blockInfo.Textures.numTextures > 0)
            {
                for (size_t i = 0; i < blockInfo.Textures.numTextures; i++)
                {
                    textureIndices[blockInfo.Textures.textureIDs[i] - 1] = s_Textures[blockInfo.Textures.textureIDs[i]];
                }
            }
        }
        return textureIndices;
    }
}// namespace Engine
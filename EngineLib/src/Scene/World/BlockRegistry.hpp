#pragma once
#include <array>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Engine
{
    namespace BlockType
    {
        inline constexpr uint32_t AIR = 0;
        inline constexpr uint32_t WOOD = 1;
        inline constexpr uint32_t DIRT = 2;
        inline constexpr uint32_t GRASS = 3;
        inline constexpr uint32_t COBBLESTONE = 4;
        inline constexpr uint32_t STONE = 5;
        inline constexpr uint32_t SAPLING = 6;
        inline constexpr uint32_t BEDROCK = 7;
        inline constexpr uint32_t WATER = 8;
        inline constexpr uint32_t SAND = 9;
        inline constexpr uint32_t MAX = 10;
    }// namespace BlockType

    namespace TextureType
    {
        inline constexpr uint32_t TOP = 0;
        inline constexpr uint32_t BOTTOM = 1 << 1;
        inline constexpr uint32_t LEFT = 1 << 2;
        inline constexpr uint32_t RIGHT = 1 << 3;
        inline constexpr uint32_t FRONT = 1 << 4;
        inline constexpr uint32_t BACK = 1 << 5;
        inline constexpr uint32_t SIDES = 1 << 6;
    }// namespace TextureType

    struct BlockTextures {
        size_t numTextures = 0;
        size_t flags = 0;
        size_t textureIDs[6] = {0, 0, 0, 0, 0, 0};
    };

    struct BlockInfo {

        uint32_t Type;
        std::string_view Name;

        BlockTextures Textures;
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

        static std::unordered_map<uint32_t, std::string> AssignTexturesToIndices();

    private:
        static std::array<BlockInfo, 256> s_BlockInfos;
        static std::unordered_map<size_t, const char*> s_Textures;
        static std::unordered_map<std::string, uint32_t> s_BlockIDs;
    };
}// namespace Engine
#include "BlockRegistry.hpp"
#include <algorithm>

std::array<std::string, 256> BlockRegistry::s_BlockNames = {};

void BlockRegistry::Init()
{
    s_BlockNames[0] = "Air";
    s_BlockNames[1] = "Stone";
    s_BlockNames[2] = "Grass";
    s_BlockNames[3] = "Dirt";
    s_BlockNames[4] = "Cobblestone";
    s_BlockNames[5] = "Wood";
    s_BlockNames[6] = "Sapling";
    s_BlockNames[7] = "Bedrock";
    s_BlockNames[8] = "Water";
    for (int i = 9; i < 256; i++) { s_BlockNames[i] = ""; }
}

void BlockRegistry::Destroy() {}

std::string BlockRegistry::GetBlockName(uint32_t id) { return s_BlockNames[id]; }

uint32_t BlockRegistry::GetBlockID(std::string_view name)
{
    auto result = std::find(s_BlockNames.begin(), s_BlockNames.end(), name);
    if (result != s_BlockNames.end()) { return std::distance(s_BlockNames.begin(), result); }
    return 0;
}
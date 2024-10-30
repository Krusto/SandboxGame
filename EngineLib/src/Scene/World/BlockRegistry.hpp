#pragma once
#include <array>
#include <string>
#include <string_view>

class BlockRegistry
{
public:
    BlockRegistry() = default;
    ~BlockRegistry() = default;

    static void Init();
    static void Destroy();

    static std::string GetBlockName(uint32_t id);
    static uint32_t GetBlockID(std::string_view name);

private:
    static std::array<std::string, 256> s_BlockNames;
};

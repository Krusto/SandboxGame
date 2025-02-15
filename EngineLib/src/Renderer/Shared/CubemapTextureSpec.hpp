#pragma once
#include <string_view>
#include <cstdint>

namespace Engine
{
    enum class CubemapTextureFace : uint8_t
    {
        Right = 0,
        Left,
        Top,
        Bottom,
        Front,
        Back
    };

    struct CubemapTextureSpec {
        std::string_view Name{};
        std::string_view WorkingDirectory{};
        int32_t Width{};
        int32_t Height{};
        int32_t Channels{};
    };
}// namespace Engine
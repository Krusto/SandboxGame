#pragma once
#include <Core/Types.hpp>

namespace Engine
{
    enum ImageColorFormat : uint8_t
    {
        RGBA = 0,
        RGB,
        RG,
        R,
        Depth
    };

    enum ImageType : uint8_t
    {
        STATIC = 0,
        DYNAMIC
    };

}// namespace Engine
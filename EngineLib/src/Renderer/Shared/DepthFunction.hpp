#pragma once
#include <cstdint>

namespace Engine
{
    enum DepthFunction
    {
        Never = 0,
        Less,
        Equal,
        LEqual,
        Greater,
        NotEqual,
        GEqual,
        Always
    };
}
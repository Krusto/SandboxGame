//
// Created by Stoyanov, Krusto (K.S.) on 6/23/2022.
//
#pragma once
#include <cstdint>

namespace Engine
{

    enum class ShaderUniformType
    {
        None = 0,
        Bool,
        Int,
        UInt,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Mat3,
        Mat4,
        IVec2,
        IVec3,
        IVec4
    };

    class ShaderDataType
    {
    public:
        ShaderDataType() = delete;
        ~ShaderDataType() = delete;

        static size_t Size(ShaderUniformType Type)
        {
            switch (Type)
            {
                case ShaderUniformType::Vec2:
                    return 2 * sizeof(float);
                case ShaderUniformType::Vec3:
                    return 3 * sizeof(float);
                case ShaderUniformType::Vec4:
                    return 4 * sizeof(float);
                case ShaderUniformType::Float:
                    return sizeof(float);
                default:
                    return 0;
            }
        }
    };
}// namespace Engine
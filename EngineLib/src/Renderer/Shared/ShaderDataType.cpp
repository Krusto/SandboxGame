#include "ShaderUniform.h"

namespace Engine
{
    ShaderUniformType ShaderDataType::ShaderDataTypeFromString(std::string_view type)
    {
        if (type == "bool") return ShaderUniformType::Bool;
        if (type == "int") return ShaderUniformType::Int;
        if (type == "uint") return ShaderUniformType::UInt;
        if (type == "float") return ShaderUniformType::Float;
        if (type == "vec2") return ShaderUniformType::Vec2;
        if (type == "vec3") return ShaderUniformType::Vec3;
        if (type == "vec4") return ShaderUniformType::Vec4;
        if (type == "mat3") return ShaderUniformType::Mat3;
        if (type == "mat4") return ShaderUniformType::Mat4;
        if (type == "ivec2") return ShaderUniformType::IVec2;
        if (type == "ivec3") return ShaderUniformType::IVec3;
        if (type == "ivec4") return ShaderUniformType::IVec4;
        return ShaderUniformType::None;
    }

    std::string ShaderDataType::ShaderBlockMemoryLayoutString(ShaderBlockMemoryLayout layout)
    {
        if (layout == ShaderBlockMemoryLayout::Packed) return "packed";
        if (layout == ShaderBlockMemoryLayout::Shared) return "shared";
        if (layout == ShaderBlockMemoryLayout::Std140) return "std140";
        if (layout == ShaderBlockMemoryLayout::Std430) return "std430";
        return {};
    }

    std::string ShaderDataType::ShaderDataTypeString(ShaderUniformType type)
    {
        switch (type)
        {
            case ShaderUniformType::Bool:
                return "bool";
            case ShaderUniformType::Int:
                return "int";
            case ShaderUniformType::UInt:
                return "uint";
            case ShaderUniformType::Float:
                return "float";
            case ShaderUniformType::Vec2:
                return "vec2";
            case ShaderUniformType::Vec3:
                return "vec3";
            case ShaderUniformType::Vec4:
                return "vec4";
            case ShaderUniformType::Mat3:
                return "mat3";
            case ShaderUniformType::Mat4:
                return "mat4";
            case ShaderUniformType::IVec2:
                return "ivec2";
            case ShaderUniformType::IVec3:
                return "ivec3";
            case ShaderUniformType::IVec4:
                return "ivec4";
            default:
                return {};
        }
    }
}// namespace Engine
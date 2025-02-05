#pragma once
#include <string>
#include <cstdint>
#include <vector>
#include <string_view>

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

    enum class ShaderBlockMemoryLayout : uint8_t
    {
        None = 0,
        Packed,
        Shared,
        Std140,
        Std430
    };

    enum ShaderType
    {
        None = 0,
        Vertex,
        Fragment,
        Compute,
        Other
    };

    class ShaderDataType
    {
    public:
        ShaderDataType() = delete;
        ~ShaderDataType() = delete;

        static ShaderUniformType ShaderDataTypeFromString(std::string_view type);

        static std::string ShaderBlockMemoryLayoutString(ShaderBlockMemoryLayout layout);

        static std::string ShaderDataTypeString(ShaderUniformType type);

        static size_t Size(ShaderUniformType Type)
        {
            switch (Type)
            {
                case ShaderUniformType::Bool:
                    return sizeof(bool);
                case ShaderUniformType::Int:
                    return sizeof(int);
                case ShaderUniformType::UInt:
                    return sizeof(uint32_t);
                case ShaderUniformType::Float:
                    return sizeof(float);
                case ShaderUniformType::Vec2:
                    return 2 * sizeof(float);
                case ShaderUniformType::Vec3:
                    return 3 * sizeof(float);
                case ShaderUniformType::Vec4:
                    return 4 * sizeof(float);
                case ShaderUniformType::Mat3:
                    return 3 * 3 * sizeof(float);
                case ShaderUniformType::Mat4:
                    return 4 * 4 * sizeof(float);
                case ShaderUniformType::IVec2:
                    return 2 * sizeof(int);
                case ShaderUniformType::IVec3:
                    return 3 * sizeof(int);
                case ShaderUniformType::IVec4:
                    return 4 * sizeof(int);
                default:
                    return 0;
            }
        }
    };

    struct ShaderUniformDeclaration {
        std::string Name{};
        uint32_t Offset{};
        ShaderUniformType Type{};
    };

    struct ShaderUniformBlockLayout {
        std::string Name{};
        uint32_t Binding{};
        uint32_t Index{};
        ShaderBlockMemoryLayout Layout{};
        size_t Alignment{};
        std::vector<ShaderUniformDeclaration> Uniforms{};
    };
}// namespace Engine
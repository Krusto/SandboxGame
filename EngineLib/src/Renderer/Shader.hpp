#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <Renderer/ShaderUniform.hpp>
#include "ShaderDataType.hpp"

namespace Engine
{
    enum class ShaderType
    {
        None,
        Vertex,
        Fragment,
        Compute,
        Other
    };

    class ShaderUniform
    {
    public:
        ShaderUniform() = default;

        ShaderUniform(std::string Name, const ShaderUniformType type, const uint32_t size, const uint32_t offset)
            : m_Name(std::move(Name)), m_Type(type), m_Size(size), m_Offset(offset)
        {}

        const auto& GetName() { return m_Name; }

        ShaderUniformType& GetType() { return m_Type; }

        const ShaderUniformType& GetType() const { return m_Type; }

        uint32_t GetSize() { return m_Size; }

        const uint32_t GetSize() const { return m_Size; }

        uint32_t GetOffset() { return m_Offset; }

        const uint32_t GetOffset() const { return m_Offset; }

        std::string_view m_Name{};
        ShaderUniformType m_Type{};
        uint32_t m_Size{};
        uint32_t m_Offset{};
    };

    struct ShaderUniformBuffer {
        std::string Name;
        uint32_t Index;
        uint32_t BindingPoint;
        uint32_t Size;
        uint32_t id;
        std::vector<ShaderUniform> Uniforms;
    };

    struct ShaderStorageBuffer {
        std::string Name;
        uint32_t Index;
        uint32_t BindingPoint;
        uint32_t Size;
        uint32_t id;
        //std::vector<ShaderUniform> Uniforms;
    };

    struct ShaderBuffer {
        std::string Name;
        uint32_t Size = 0;
        std::unordered_map<std::string, ShaderUniform> Uniforms;
    };

    class Shader
    {
    public:
        static Shader* Load(const std::string& path);

    public:
        virtual ~Shader() = default;

        virtual void Destroy() = 0;
        virtual std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffers() = 0;
        virtual const std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffers() const = 0;
        virtual const std::unordered_map<std::string, ShaderResourceDeclaration>& GetResources() const = 0;
        virtual const std::string& GetName() const = 0;
        virtual const std::string& GetPath() const = 0;
        virtual void Reload(bool forceCompile = true) = 0;
        virtual void Bind() = 0;
        virtual void SetUniformBuffer(const std::string& name, const void* data, uint32_t size) = 0;
        virtual void SetStorageBuffer(const std::string& name, const void* data, uint32_t size) = 0;
        virtual void ResizeStorageBuffer(uint32_t bindingPoint, uint32_t newSize) = 0;
        virtual void SetUniform(const std::string& fullname, float value) = 0;
        virtual void SetUniform(const std::string& fullname, int value) = 0;
        virtual void SetUniform(const std::string& fullname, const glm::ivec2& value) = 0;
        virtual void SetUniform(const std::string& fullname, const glm::ivec3& value) = 0;
        virtual void SetUniform(const std::string& fullname, const glm::ivec4& value) = 0;
        virtual void SetUniform(const std::string& fullname, uint32_t value) = 0;
        virtual void SetUniform(const std::string& fullname, const glm::vec2& value) = 0;
        virtual void SetUniform(const std::string& fullname, const glm::vec3& value) = 0;
        virtual void SetUniform(const std::string& fullname, const glm::vec4& value) = 0;
        virtual void SetUniform(const std::string& fullname, const glm::mat3& value) = 0;
        virtual void SetUniform(const std::string& fullname, const glm::mat4& value) = 0;
        virtual void AddResource(std::string_view name, ShaderResourceDeclaration resource) = 0;

    public:
        uint32_t offset = 0;
    };

}// namespace Engine
#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include <Core/Ref.hpp>

#include <Renderer/RendererCommand.hpp>
#include <Renderer/ShaderUniform.hpp>
#include <Renderer/ShaderDataType.hpp>

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

    class Shader: public RefCounted
    {
    public:
        static Shader* Load(const std::string& path);

    public:
        virtual ~Shader() = default;

        virtual void Destroy() = 0;
        virtual const std::string& GetName() const = 0;
        virtual const std::string& GetPath() const = 0;
        virtual void Reload(bool forceCompile = true) = 0;
        virtual void Bind() = 0;
        virtual void Bind() const = 0;
        virtual void SetUniform(const std::string& fullname, float value) const = 0;
        virtual void SetUniform(const std::string& fullname, int value) const = 0;
        virtual void SetUniform(const std::string& fullname, const glm::ivec2& value) const = 0;
        virtual void SetUniform(const std::string& fullname, const glm::ivec3& value) const = 0;
        virtual void SetUniform(const std::string& fullname, const glm::ivec4& value) const = 0;
        virtual void SetUniform(const std::string& fullname, uint32_t value) const = 0;
        virtual void SetUniform(const std::string& fullname, const glm::vec2& value) const = 0;
        virtual void SetUniform(const std::string& fullname, const glm::vec3& value) const = 0;
        virtual void SetUniform(const std::string& fullname, const glm::vec4& value) const = 0;
        virtual void SetUniform(const std::string& fullname, const glm::mat3& value) const = 0;
        virtual void SetUniform(const std::string& fullname, const glm::mat4& value) const = 0;

    public:
        RendererCommand BindCommand() const;

    public:
        uint32_t offset = 0;
    };

}// namespace Engine
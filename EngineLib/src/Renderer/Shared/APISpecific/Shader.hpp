#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include <Renderer/Predefines.hpp>
#include <Renderer/Shared/RendererCommand.hpp>
#include <Renderer/Shared/ShaderUniform.hpp>

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
    struct ShaderData;

    class EXPORT_RENDERER Shader
    {
    public:
        Shader() = default;
        ~Shader() = default;
        static Shader Create(const std::string& path);

    public:
        void Load(const std::string& path);
        void Destroy();
        const std::string& GetName() const;
        const std::string& GetPath() const;
        void Reload(bool forceCompile = true);
        void Bind();
        void Bind() const;
        uint32_t ID() const;
        void SetUniform(const std::string& fullname, float value) const;
        void SetUniform(const std::string& fullname, int value) const;
        void SetUniform(const std::string& fullname, const glm::ivec2& value) const;
        void SetUniform(const std::string& fullname, const glm::ivec3& value) const;
        void SetUniform(const std::string& fullname, const glm::ivec4& value) const;
        void SetUniform(const std::string& fullname, uint32_t value) const;
        void SetUniform(const std::string& fullname, const glm::vec2& value) const;
        void SetUniform(const std::string& fullname, const glm::vec3& value) const;
        void SetUniform(const std::string& fullname, const glm::vec4& value) const;
        void SetUniform(const std::string& fullname, const glm::mat3& value) const;
        void SetUniform(const std::string& fullname, const glm::mat4& value) const;

    public:
        RendererCommand BindCommand() const;

    private:
        ShaderData* m_Data{};
    };

}// namespace Engine
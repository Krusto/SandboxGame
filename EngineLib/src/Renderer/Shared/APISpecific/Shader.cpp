#include "Shader.hpp"
#include <Renderer/Shared/function_pointers.h>

namespace Engine
{

    Shader Shader::Create(const std::string& path)
    {
        Shader ptr;
        ptr.Load(path);
        return ptr;
    }

    void Shader::Load(const std::string& path) { ShaderLoad((void**) &m_Data, path.c_str()); }

    void Shader::Destroy() { ShaderDestroy((void**) &m_Data); }

    const std::string& Shader::GetName() const { return std::string(ShaderGetName((void*) m_Data)); }

    const std::string& Shader::GetPath() const { return std::string(ShaderGetPath((void*) m_Data)); }

    void Shader::Reload(bool forceCompile) { ShaderReload((void*) m_Data, forceCompile); }

    void Shader::Bind() { ShaderBind((void*) m_Data); }

    void Shader::Bind() const { ShaderBind((void*) m_Data); }

    uint32_t Shader::ID() const { return ShaderGetID((void*) m_Data); }

    void Shader::SetUniform(const std::string& fullname, float value) const
    {
        ShaderSetUniformF((void*) m_Data, fullname.c_str(), value);
    }

    void Shader::SetUniform(const std::string& fullname, int value) const
    {
        ShaderSetUniformI((void*) m_Data, fullname.c_str(), value);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::ivec2& value) const
    {
        ShaderSetUniform2I((void*) m_Data, fullname.c_str(), value.x, value.y);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::ivec3& value) const
    {
        ShaderSetUniform3I((void*) m_Data, fullname.c_str(), value.x, value.y, value.z);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::ivec4& value) const
    {
        ShaderSetUniform4I((void*) m_Data, fullname.c_str(), value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniform(const std::string& fullname, uint32_t value) const
    {
        ShaderSetUniformUI((void*) m_Data, fullname.c_str(), value);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::vec2& value) const
    {
        ShaderSetUniform2F((void*) m_Data, fullname.c_str(), value.x, value.y);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::vec3& value) const
    {
        ShaderSetUniform3F((void*) m_Data, fullname.c_str(), value.x, value.y, value.z);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::vec4& value) const
    {
        ShaderSetUniform4F((void*) m_Data, fullname.c_str(), value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::mat3& value) const
    {
        ShaderSetUniformMat3((void*) m_Data, fullname.c_str(), (void*) &value);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::mat4& value) const
    {
        ShaderSetUniformMat4((void*) m_Data, fullname.c_str(), (void*) &value);
    }

    RendererCommand Shader::BindCommand() const
    {
        return RendererCommand([=]() { Bind(); });
    }
}// namespace Engine
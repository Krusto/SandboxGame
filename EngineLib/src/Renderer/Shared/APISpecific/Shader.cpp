#include "Shader.hpp"
#include "Renderer/Renderer.hpp"
namespace Engine
{

    Shader Shader::Create(const std::string& path)
    {
        Shader ptr;
        ptr.Load(path);
        return ptr;
    }

    void Shader::Load(const std::string& path) { Renderer::GetInstance()->ShaderLoad((void**) &m_Data, path.c_str()); }

    void Shader::Destroy() { Renderer::GetInstance()->ShaderDestroy((void**) &m_Data); }

    const std::string& Shader::GetName() const
    {
        return std::string(Renderer::GetInstance()->ShaderGetName((void*) m_Data));
    }

    const std::string& Shader::GetPath() const
    {
        return std::string(Renderer::GetInstance()->ShaderGetPath((void*) m_Data));
    }

    void Shader::Reload(bool forceCompile) { Renderer::GetInstance()->ShaderReload((void*) m_Data, forceCompile); }

    void Shader::Bind() { Renderer::GetInstance()->ShaderBind((void*) m_Data); }

    void Shader::Bind() const { Renderer::GetInstance()->ShaderBind((void*) m_Data); }

    uint32_t Shader::ID() const { return Renderer::GetInstance()->ShaderGetID((void*) m_Data); }

    void Shader::SetUniform(const std::string& fullname, float value) const
    {
        Renderer::GetInstance()->ShaderSetUniformF((void*) m_Data, fullname.c_str(), value);
    }

    void Shader::SetUniform(const std::string& fullname, int value) const
    {
        Renderer::GetInstance()->ShaderSetUniformI((void*) m_Data, fullname.c_str(), value);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::ivec2& value) const
    {
        Renderer::GetInstance()->ShaderSetUniform2I((void*) m_Data, fullname.c_str(), value.x, value.y);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::ivec3& value) const
    {
        Renderer::GetInstance()->ShaderSetUniform3I((void*) m_Data, fullname.c_str(), value.x, value.y, value.z);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::ivec4& value) const
    {
        Renderer::GetInstance()->ShaderSetUniform4I((void*) m_Data, fullname.c_str(), value.x, value.y, value.z,
                                                    value.w);
    }

    void Shader::SetUniform(const std::string& fullname, uint32_t value) const
    {
        Renderer::GetInstance()->ShaderSetUniformUI((void*) m_Data, fullname.c_str(), value);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::vec2& value) const
    {
        Renderer::GetInstance()->ShaderSetUniform2F((void*) m_Data, fullname.c_str(), value.x, value.y);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::vec3& value) const
    {
        Renderer::GetInstance()->ShaderSetUniform3F((void*) m_Data, fullname.c_str(), value.x, value.y, value.z);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::vec4& value) const
    {
        Renderer::GetInstance()->ShaderSetUniform4F((void*) m_Data, fullname.c_str(), value.x, value.y, value.z,
                                                    value.w);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::mat3& value) const
    {
        Renderer::GetInstance()->ShaderSetUniformMat3((void*) m_Data, fullname.c_str(), (void*) &value);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::mat4& value) const
    {
        Renderer::GetInstance()->ShaderSetUniformMat4((void*) m_Data, fullname.c_str(), (void*) &value);
    }

    RendererCommand Shader::BindCommand() const
    {
        return RendererCommand([=]() { Bind(); });
    }
}// namespace Engine
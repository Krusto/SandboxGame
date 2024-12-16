#pragma once
#include <glad/glad.h>

#include <Core/Log.hpp>

#include <Renderer/Shader.hpp>
#include <Renderer/ShaderUniform.hpp>

namespace Engine
{
    struct ShaderData {
        uint32_t rendererID = 0;
        bool loaded = false;
        bool compiled = false;

        std::string name, assetPath;

        std::unordered_map<GLenum, std::string> shaderSource;
        std::vector<ShaderUniformBlockLayout> uniformBlocks;
    };

    class OpenGLShader: public Shader

    {
    public:
        OpenGLShader() = default;
        OpenGLShader(const std::string& filepath, bool forceRecompile);

        ~OpenGLShader() = default;

    public:
        static OpenGLShader* CreateFromString(const std::string& source);

    public:
        virtual void Destroy() override;

        virtual const std::string& GetName() const override { return m_Name; }

        virtual const std::string& GetPath() const override { return m_AssetPath; }

        virtual void Reload(bool forceCompile = true) override;

        virtual void Bind() override;
        virtual void Bind() const override;

        virtual uint32_t ID() const override;

        virtual void SetUniform(const std::string& fullname, float value) const override;
        virtual void SetUniform(const std::string& fullname, int value) const override;
        virtual void SetUniform(const std::string& fullname, const glm::ivec2& value) const override;
        virtual void SetUniform(const std::string& fullname, const glm::ivec3& value) const override;
        virtual void SetUniform(const std::string& fullname, const glm::ivec4& value) const override;
        virtual void SetUniform(const std::string& fullname, uint32_t value) const override;
        virtual void SetUniform(const std::string& fullname, const glm::vec2& value) const override;
        virtual void SetUniform(const std::string& fullname, const glm::vec3& value) const override;
        virtual void SetUniform(const std::string& fullname, const glm::vec4& value) const override;
        virtual void SetUniform(const std::string& fullname, const glm::mat3& value) const override;
        virtual void SetUniform(const std::string& fullname, const glm::mat4& value) const override;

    private:
        void Load(const std::string& source, bool forceCompile);

        void Compile(const std::string& vertexData, const std::string& fragmentData);

        void CheckShader(GLuint id, GLuint type, GLint* ret, const char* onfail);

        std::string ReadShaderFromFile(const std::string& filepath, ShaderType shaderType) const;

        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

        int32_t GetUniformLocation(const std::string& name) const;

    private:
        uint32_t m_RendererID = 0;
        bool m_Loaded = false;
        bool m_Compiled = false;

        std::string m_Name, m_AssetPath;

        std::unordered_map<GLenum, std::string> m_ShaderSource;
        std::vector<ShaderUniformBlockLayout> m_UniformBlocks;
    };

}// namespace Engine
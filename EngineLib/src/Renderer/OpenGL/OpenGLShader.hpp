#pragma once
#include <glad/glad.h>

#include <Core/Log.hpp>

#include <Renderer/Shader.hpp>
#include <Renderer/ShaderUniform.hpp>

namespace Engine
{
    class OpenGLShader: public Shader
    {
    public:
        OpenGLShader();
        OpenGLShader(const std::string& filepath, bool forceRecompile);

        ~OpenGLShader() { LOG_INFO("Destroying Shader") }

    public:
        virtual void Destroy() override;

        static Ref<OpenGLShader> CreateFromString(const std::string& source);

        virtual void Reload(bool forceCompile = true) override;

        virtual void Bind() override;

        static const ShaderUniformBuffer& FindUniformBuffer(const std::string& name) { return s_UniformBuffers[0]; }

        static void SetUniformBuffer(const ShaderUniformBuffer& buffer, const void* data, uint32_t size,
                                     uint32_t offset = 0)
        {}

        static void SetStorageBuffer(const ShaderStorageBuffer& buffer, const void* data, uint32_t size,
                                     uint32_t offset = 0)
        {}

        virtual void SetUniformBuffer(const std::string& name, const void* data, uint32_t size) override {}

        virtual void SetStorageBuffer(const std::string& name, const void* data, uint32_t size) override {}

        virtual void ResizeStorageBuffer(uint32_t bindingPoint, uint32_t newSize) override {}

        virtual void SetUniform(const std::string& fullname, float value) override;

        virtual void SetUniform(const std::string& fullname, int value) override;

        virtual void SetUniform(const std::string& fullname, const glm::ivec2& value) override;

        virtual void SetUniform(const std::string& fullname, const glm::ivec3& value) override;

        virtual void SetUniform(const std::string& fullname, const glm::ivec4& value) override;

        virtual void SetUniform(const std::string& fullname, uint32_t value) override;

        virtual void SetUniform(const std::string& fullname, const glm::vec2& value) override;

        virtual void SetUniform(const std::string& fullname, const glm::vec3& value) override;

        virtual void SetUniform(const std::string& fullname, const glm::vec4& value) override;

        virtual void SetUniform(const std::string& fullname, const glm::mat3& value) override;

        virtual void SetUniform(const std::string& fullname, const glm::mat4& value) override;

        virtual const std::string& GetName() const override { return m_Name; }

        virtual const std::string& GetPath() const override { return m_AssetPath; }

        virtual std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffers() override { return m_Buffers; }

        virtual const std::unordered_map<std::string, ShaderBuffer>& GetShaderBuffers() const override
        {
            return m_Buffers;
        }

        virtual const std::unordered_map<std::string, ShaderResourceDeclaration>& GetResources() const override
        {
            return m_Resources;
        }

        const ShaderResourceDeclaration* GetShaderResource(const std::string& name);

        static void ClearUniformBuffers();

        void Load(const std::string& source, bool forceCompile);

        virtual void AddResource(std::string_view name, ShaderResourceDeclaration resource) override;

    private:
        void Compile(const std::string& vertexData, const std::string& fragmentData);

        void Reflect(std::vector<uint32_t>& data) {}

        void CheckShader(GLuint id, GLuint type, GLint* ret, const char* onfail);

        void CompileOrGetVulkanBinary(std::unordered_map<uint32_t, std::vector<uint32_t>>& outputBinary,
                                      bool forceCompile = false)
        {}

        void CompileOrGetOpenGLBinary(const std::unordered_map<uint32_t, std::vector<uint32_t>>&,
                                      bool forceCompile = false)
        {}

        std::string ReadShaderFromFile(const std::string& filepath, ShaderType shaderType) const;

        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

        int32_t GetUniformLocation(const std::string& name) const;

        static GLenum ShaderTypeFromString(const std::string& type)
        {
            if (type == "vertex") return GL_VERTEX_SHADER;
            if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
            if (type == "compute") return GL_COMPUTE_SHADER;

            return GL_NONE;
        }

        void UploadUniformInt(uint32_t location, int32_t value);

        void UploadUniformIntArray(uint32_t location, int32_t* values, int32_t count);

        void UploadUniformFloat(uint32_t location, float value);

        void UploadUniformFloat2(uint32_t location, const glm::vec2& value);

        void UploadUniformFloat3(uint32_t location, const glm::vec3& value);

        void UploadUniformFloat4(uint32_t location, const glm::vec4& value);

        void UploadUniformMat3(uint32_t location, const glm::mat3& values);

        void UploadUniformMat4(uint32_t location, const glm::mat4& values);

        void UploadUniformMat4Array(uint32_t location, const glm::mat4& values, uint32_t count);

        void UploadUniformInt(const std::string& name, int32_t value);

        void UploadUniformUInt(const std::string& name, uint32_t value);

        void UploadUniformIntArray(const std::string& name, int32_t* values, uint32_t count);

        void UploadUniformFloat(const std::string& name, float value);

        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);

        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);

        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void UploadUniformMat4(const std::string& name, const glm::mat4& value);

    private:
        uint32_t m_RendererID = 0;
        bool m_Loaded = false;
        bool m_IsCompute = false;

        uint32_t m_ConstantBufferOffset = 0;

        std::string m_Name, m_AssetPath;
        std::unordered_map<GLenum, std::string> m_ShaderSource;

        inline static std::unordered_map<uint32_t, ShaderUniformBuffer> s_UniformBuffers;
        inline static std::unordered_map<uint32_t, ShaderStorageBuffer> s_StorageBuffers;

        std::unordered_map<std::string, ShaderBuffer> m_Buffers;
        std::unordered_map<std::string, ShaderResourceDeclaration> m_Resources;
        std::unordered_map<std::string, GLint> m_UniformLocations;

        bool m_Compiled = false;
    };

}// namespace Engine
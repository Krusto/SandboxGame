#include <glad/glad.h>
#include <Renderer/Renderer.hpp>
#include <Renderer/Shared/APISpecific/Shader.hpp>
#include <Renderer/Shared/ShaderUniform.hpp>
#include <Util/File.hpp>
#include <Core/Allocator.hpp>
#include <string>
#include <sstream>
#include <limits>
#include <fstream>
#include <filesystem>

#include <glm/gtc/type_ptr.hpp>

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

    void Compile(ShaderData* shaderData, const std::string& vertexData, const std::string& fragmentData);

    void CheckShader(ShaderData* shaderData, GLuint id, GLuint type, GLint* ret, const char* onfail);

    std::string ReadShaderFromFile(ShaderData* shaderData, const std::string& filepath, ShaderType shaderType);

    std::unordered_map<GLenum, std::string> PreProcess(ShaderData* shaderData, const std::string& source);

    int32_t GetUniformLocation(ShaderData* shaderData, const std::string& name);

    void Shader::Reload(bool forceCompile)
    {
        m_Data->shaderSource =
                PreProcess(m_Data, std::filesystem::path(m_Data->assetPath).append(m_Data->name).string());

        Compile(m_Data, m_Data->shaderSource[GL_VERTEX_SHADER], m_Data->shaderSource[GL_FRAGMENT_SHADER]);
    }

    void Shader::Load(const std::string& path)
    {
        if (m_Data == nullptr) { m_Data = Engine::Allocator::Allocate<ShaderData>(); }
        m_Data->assetPath = std::filesystem::absolute(path).parent_path().string();
        m_Data->name = std::filesystem::path(path).filename().string();
        Reload(true);
    }

    void SubmitShader(Shader* shader, const std::string& vertexData, const std::string& fragmentData) {}

    void Compile(ShaderData* shaderData, const std::string& vertexData, const std::string& fragmentData)
    {

        Renderer::SubmitAndFlush(std::function<void(void)>([&]() {
            if (shaderData->rendererID != 0) glDeleteProgram(shaderData->rendererID);

            const char* vShaderCode = vertexData.c_str();
            const char* fShaderCode = fragmentData.c_str();
            uint32_t vertex, fragment;
            GLint result;
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, nullptr);
            glCompileShader(vertex);
            CheckShader(shaderData, vertex, GL_COMPILE_STATUS, &result, "Unable to compile the vertex shader!");

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, nullptr);
            glCompileShader(fragment);
            CheckShader(shaderData, fragment, GL_COMPILE_STATUS, &result, "Unable to compile the fragment shader!");

            shaderData->rendererID = glCreateProgram();
            glAttachShader(shaderData->rendererID, vertex);
            glAttachShader(shaderData->rendererID, fragment);
            glLinkProgram(shaderData->rendererID);
            CheckShader(shaderData, shaderData->rendererID, GL_LINK_STATUS, &result, "Unable to link the program!");

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }));
    }

    void CheckShader(ShaderData* shaderData, GLuint id, GLuint type, GLint* ret, const char* onfail)
    {
        switch (type)
        {
            case (GL_COMPILE_STATUS):
                glGetShaderiv(id, type, ret);
                break;
            case (GL_LINK_STATUS):
                glGetProgramiv(id, type, ret);
                break;
        };

        if (*ret == false)
        {
            int infologLength = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infologLength);
            std::vector<GLchar> buffer(infologLength);
            GLsizei charsWritten = 0;
            std::cout << onfail << std::endl;
            glGetShaderInfoLog(id, infologLength, &charsWritten, buffer.data());

            LOG_ERROR("%s\n", buffer.data());
            // std::cout << buffer.data() << std::endl;
        }
    }

    std::unordered_map<GLenum, std::string> PreProcess(ShaderData* shaderData, const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        shaderSources[GL_VERTEX_SHADER] = ReadShaderFromFile(shaderData, source, ShaderType::Vertex);
        shaderSources[GL_FRAGMENT_SHADER] = ReadShaderFromFile(shaderData, source, ShaderType::Fragment);

        return shaderSources;
    }

    void Shader::Bind() { glUseProgram(m_Data->rendererID); }

    void Shader::Bind() const { glUseProgram(m_Data->rendererID); }

    uint32_t Shader::ID() const { return m_Data->rendererID; }

    static bool file_read_string(const int8_t* path, size_t* len, std::string* buffer)
    {
        std::ifstream f(std::string((const char*) path), std::ios::in);
        if (f)
        {
            f.seekg(0, std::ios::end);
            *len = f.tellg();
            f.seekg(0, std::ios::beg);
            buffer->resize(*len, '\n');
            f.read((char*) buffer->data(), *len);
            f.close();
            return buffer->size() > 0;
        }
        return false;
    }

    std::string ReadShaderFromFile(ShaderData* shaderData, const std::string& filepath, ShaderType shaderType)
    {
        std::string result{};
        std::ifstream in;

        std::ios_base::iostate exceptionMask = in.exceptions() | std::ios::failbit;
        in.exceptions(exceptionMask);

        try
        {
            std::string buffer;
            size_t bufferLen;
            std::string path;
            std::unordered_map<ShaderType, std::string> extensions;

            extensions[ShaderType::Vertex] = ".vert.glsl";
            extensions[ShaderType::Fragment] = ".frag.glsl";
            extensions[ShaderType::Compute] = ".comp.glsl";

            path = filepath + extensions[shaderType];

            auto resultStatus = file_read_string((const int8_t*) path.c_str(), &bufferLen, &buffer);
            if (resultStatus != false) { result = buffer; }
        }
        catch (std::ios_base::failure& e)
        {
            LOG_ERROR("%s %s\n", e.what(), filepath.c_str());
        }
        return result;
    }

    void Shader::Destroy()
    {
        if (m_Data)
        {
            glDeleteProgram(m_Data->rendererID);
            Allocator::Deallocate(m_Data);
            m_Data = nullptr;
        }
    }

    int32_t GetUniformLocation(ShaderData* shaderData, const std::string& name)
    {
        int32_t result = glGetUniformLocation(shaderData->rendererID, name.c_str());
        return result;
    }

    void Shader::SetUniform(const std::string& fullname, float value) const
    {
        glUniform1f(GetUniformLocation(m_Data, fullname), value);
    }

    void Shader::SetUniform(const std::string& fullname, int value) const
    {
        glUniform1i(GetUniformLocation(m_Data, fullname), value);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::ivec2& value) const
    {
        glUniform2i(GetUniformLocation(m_Data, fullname), value.x, value.y);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::ivec3& value) const
    {
        glUniform3i(GetUniformLocation(m_Data, fullname), value.x, value.y, value.z);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::ivec4& value) const
    {
        glUniform4i(GetUniformLocation(m_Data, fullname), value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniform(const std::string& fullname, uint32_t value) const
    {
        glUniform1ui(GetUniformLocation(m_Data, fullname), value);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::vec2& value) const
    {
        glUniform2f(GetUniformLocation(m_Data, fullname), value.x, value.y);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::vec3& value) const
    {
        glUniform3f(GetUniformLocation(m_Data, fullname), value.x, value.y, value.z);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::vec4& value) const
    {
        glUniform4f(GetUniformLocation(m_Data, fullname), value.x, value.y, value.z, value.w);
    }

    void Shader::SetUniform(const std::string& fullname, const glm::mat3& value) const
    {
        glUniformMatrix3fv(GetUniformLocation(m_Data, fullname), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::SetUniform(const std::string& fullname, const glm::mat4& value) const
    {
        glUniformMatrix4fv(GetUniformLocation(m_Data, fullname), 1, GL_FALSE, glm::value_ptr(value));
    }

    const std::string& Shader::GetName() const { return m_Data->name; }

    const std::string& Shader::GetPath() const { return m_Data->assetPath; };
}// namespace Engine
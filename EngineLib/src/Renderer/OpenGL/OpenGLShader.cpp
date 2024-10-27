#include "OpenGLShader.hpp"
#include <Renderer/Renderer.hpp>
#include <Util/File.hpp>

#include <string>
#include <sstream>
#include <limits>
#include <fstream>

#ifdef __cplusplus
extern "C" {
#endif
#define NO_STD_LOG
#include <CFilesystem.h>

#ifdef __cplusplus
}
#endif

#include <glm/gtc/type_ptr.hpp>

namespace Engine
{

    OpenGLShader::OpenGLShader(const std::string& filepath, bool forceRecompile)
    {
        m_AssetPath = std::filesystem::absolute(filepath).parent_path().string();
        m_Name = std::filesystem::path(filepath).filename().string();
        Reload(true);
    }

    OpenGLShader* OpenGLShader::CreateFromString(const std::string& source)
    {
        OpenGLShader* shader = new OpenGLShader();
        shader->Load(source, true);
        return shader;
    }

    void OpenGLShader::Reload(bool forceCompile)
    {
        m_ShaderSource = PreProcess(std::filesystem::path(m_AssetPath).append(m_Name).string());

        Compile(m_ShaderSource[GL_VERTEX_SHADER], m_ShaderSource[GL_FRAGMENT_SHADER]);
    }

    void OpenGLShader::Load(const std::string& source, bool forceCompile)
    {
        m_ShaderSource = PreProcess(source);
        Compile(m_ShaderSource[GL_VERTEX_SHADER], m_ShaderSource[GL_FRAGMENT_SHADER]);
    }

    void SubmitShader(OpenGLShader* shader, const std::string& vertexData, const std::string& fragmentData) {}

    void OpenGLShader::Compile(const std::string& vertexData, const std::string& fragmentData)
    {

        Renderer::SubmitAndFlush(std::function<void(void)>([&]() {
            if (this->m_RendererID != 0) glDeleteProgram(this->m_RendererID);

            const char* vShaderCode = vertexData.c_str();
            const char* fShaderCode = fragmentData.c_str();
            uint32_t vertex, fragment;
            GLint result;
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, nullptr);
            glCompileShader(vertex);
            CheckShader(vertex, GL_COMPILE_STATUS, &result, "Unable to compile the vertex shader!");

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, nullptr);
            glCompileShader(fragment);
            CheckShader(fragment, GL_COMPILE_STATUS, &result, "Unable to compile the fragment shader!");

            this->m_RendererID = glCreateProgram();
            glAttachShader(this->m_RendererID, vertex);
            glAttachShader(this->m_RendererID, fragment);
            glLinkProgram(this->m_RendererID);
            CheckShader(this->m_RendererID, GL_LINK_STATUS, &result, "Unable to link the program!");

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }));
    }

    void OpenGLShader::CheckShader(GLuint id, GLuint type, GLint* ret, const char* onfail)
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
            std::cout << buffer.data() << std::endl;
        }
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        shaderSources[GL_VERTEX_SHADER] = ReadShaderFromFile(source, ShaderType::Vertex);
        shaderSources[GL_FRAGMENT_SHADER] = ReadShaderFromFile(source, ShaderType::Fragment);

        return shaderSources;
    }

    void OpenGLShader::Bind() { glUseProgram(m_RendererID); }

    std::string OpenGLShader::ReadShaderFromFile(const std::string& filepath, ShaderType shaderType) const
    {
        std::string result{};
        std::ifstream in;

        std::ios_base::iostate exceptionMask = in.exceptions() | std::ios::failbit;
        in.exceptions(exceptionMask);

        try
        {
            int8_t* buffer;
            size_t bufferLen;
            std::string path;
            std::unordered_map<ShaderType, std::string> extensions;

            extensions[ShaderType::Vertex] = ".vert.glsl";
            extensions[ShaderType::Fragment] = ".frag.glsl";
            extensions[ShaderType::Compute] = ".comp.glsl";

            path = filepath + extensions[shaderType];

            auto resultStatus = file_read_string((const int8_t*) path.c_str(), &bufferLen, &buffer);
            if (resultStatus == FILE_READ_SUCCESFULLY)
            {
                result.resize(bufferLen);
                result.assign((const char*) buffer, bufferLen);
                free(buffer);
            }
        }
        catch (std::ios_base::failure& e)
        {
            LOG_ERROR("%s %s", e.what(), filepath.c_str());
        }
        return result;
    }

    void OpenGLShader::Destroy() { glDeleteProgram(m_RendererID); }

    int32_t OpenGLShader::GetUniformLocation(const std::string& name) const
    {
        int32_t result = glGetUniformLocation(m_RendererID, name.c_str());
        return result;
    }

    void OpenGLShader::SetUniform(const std::string& fullname, float value)
    {
        glUniform1f(GetUniformLocation(fullname), value);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, int value)
    {
        glUniform1i(GetUniformLocation(fullname), value);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::ivec2& value)
    {
        glUniform2i(GetUniformLocation(fullname), value.x, value.y);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::ivec3& value)
    {
        glUniform3i(GetUniformLocation(fullname), value.x, value.y, value.z);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::ivec4& value)
    {
        glUniform4i(GetUniformLocation(fullname), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, uint32_t value)
    {
        glUniform1ui(GetUniformLocation(fullname), value);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::vec2& value)
    {
        glUniform2f(GetUniformLocation(fullname), value.x, value.y);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::vec3& value)
    {
        glUniform3f(GetUniformLocation(fullname), value.x, value.y, value.z);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::vec4& value)
    {
        glUniform4f(GetUniformLocation(fullname), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::mat3& value)
    {
        glUniformMatrix3fv(GetUniformLocation(fullname), 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::mat4& value)
    {
        glUniformMatrix4fv(GetUniformLocation(fullname), 1, GL_FALSE, glm::value_ptr(value));
    }
}// namespace Engine
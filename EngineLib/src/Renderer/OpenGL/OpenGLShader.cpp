#include "OpenGLShader.hpp"
#include "Util/File.hpp"

#include <Renderer/Renderer.h>
#include <string>
#include <sstream>
#include <limits>

#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>


#define PRINT_SHADERS 1

namespace Utils
{

    static const char* GetCacheDirectory()
    {
        // TODO: make sure the assets directory is valid
        return "./assets/cache/shader/opengl";
    }

    static void CreateCacheDirectoryIfNeeded()
    {
        std::string cacheDirectory = GetCacheDirectory();
        if (!std::filesystem::exists(cacheDirectory)) std::filesystem::create_directories(cacheDirectory);
    }

}// namespace Utils

namespace Engine
{

    OpenGLShader::OpenGLShader(const std::string& filepath, bool forceRecompile)
    {
        m_AssetPath = std::filesystem::absolute(filepath).parent_path().string();
        m_Name = std::filesystem::path(filepath).filename().string();
        Utils::CreateCacheDirectoryIfNeeded();
        Reload(true);
    }

    OpenGLShader::OpenGLShader() {}

    Ref<OpenGLShader> OpenGLShader::CreateFromString(const std::string& source)
    {
        Ref<OpenGLShader> shader = Ref<OpenGLShader>::Create();
        shader->Load(source, true);
        return shader;
    }

    void OpenGLShader::Reload(bool forceCompile)
    {
        std::string vertexSource =
                ReadShaderFromFile(std::filesystem::path(m_AssetPath).append(m_Name).string(), ShaderType::Vertex);
        std::string fragmentSource =
                ReadShaderFromFile(std::filesystem::path(m_AssetPath).append(m_Name).string(), ShaderType::Fragment);

        m_ShaderSource[GL_VERTEX_SHADER] = vertexSource;
        m_ShaderSource[GL_FRAGMENT_SHADER] = fragmentSource;

        /*std::string source = "#type vertex\n\r" + vertexSource + "\n\r#type fragment\n\r" + fragmentSource + "\n\r";

        if(forceCompile)
            std::cout << "Compiling :" << m_AssetPath << std::endl;
        Load(source, forceCompile);*/
        if (forceCompile) Compile(m_ShaderSource[GL_VERTEX_SHADER], m_ShaderSource[GL_FRAGMENT_SHADER]);

        m_Buffers.emplace("shaderBuffer", ShaderBuffer{"shaderBuffer", 0, {}});
    }

    void OpenGLShader::Load(const std::string& source, bool forceCompile)
    {
        if (forceCompile)
        {
            m_ShaderSource = PreProcess(source);
            //Utils::CreateCacheDirectoryIfNeeded();
            Compile(m_ShaderSource[GL_VERTEX_SHADER], m_ShaderSource[GL_FRAGMENT_SHADER]);
        }
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
            CheckShader(vertex, GL_COMPILE_STATUS, &result, "unable to compile the vertex shader!");

            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, nullptr);
            glCompileShader(fragment);
            CheckShader(fragment, GL_COMPILE_STATUS, &result, "unable to compile the fragment shader!");

            this->m_RendererID = glCreateProgram();
            glAttachShader(this->m_RendererID, vertex);
            glAttachShader(this->m_RendererID, fragment);
            glLinkProgram(this->m_RendererID);
            CheckShader(this->m_RendererID, GL_LINK_STATUS, &result, "unable to link the program!");

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
                if (*ret == false)
                {
                    int infologLength = 0;
                    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLength);
                    std::vector<GLchar> buffer(infologLength);
                    GLsizei charsWritten = 0;
                    std::cout << onfail << std::endl;
                    glGetShaderInfoLog(id, infologLength, &charsWritten, buffer.data());
                    std::cout << buffer.data() << std::endl;
                }
                break;
            case (GL_LINK_STATUS):
                glGetProgramiv(id, type, ret);
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
                break;
        };
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos);
            assert(eol != std::string::npos);// "Syntax error");
            size_t begin = pos + typeTokenLength + 1;
            std::string type = source.substr(begin, eol - begin);
            assert(type == "vertex" || type == "fragment" || type == "pixel" ||
                   type == "compute");// "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            pos = source.find(typeToken, nextLinePos);
            auto shaderType = ShaderTypeFromString(type);
            shaderSources[shaderType] = source.substr(
                    nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));

            if (shaderType == GL_COMPUTE_SHADER)
            {
                m_IsCompute = true;
                break;
            }
        }

        return shaderSources;
    }

    void OpenGLShader::ClearUniformBuffers() { s_UniformBuffers.clear(); }

    void OpenGLShader::Bind() { glUseProgram(m_RendererID); }

    std::string OpenGLShader::ReadShaderFromFile(const std::string& filepath, ShaderType shaderType) const
    {
        std::string result{};
        std::ifstream in;

        std::ios_base::iostate exceptionMask = in.exceptions() | std::ios::failbit;
        in.exceptions(exceptionMask);

        try
        {
            File file;
            std::string path;
            switch (shaderType)
            {
                case ShaderType::Vertex: {
                    path = filepath + ".vert.glsl";
                    break;
                }
                case ShaderType::Fragment: {
                    path = filepath + ".frag.glsl";
                    break;
                }
                case ShaderType::Compute: {
                    path = filepath + ".comp.glsl";
                    break;
                }
                case ShaderType::None: {
                    break;
                }
                case ShaderType::Other: {
                    break;
                }
            }
            file.Init(path.c_str(), FileFormat::Regular);
            result = file.ReadTextData();
        }
        catch (std::ios_base::failure& e)
        {
            LOG_ERROR(std::string(strerror(errno)) + " " + filepath)
        }
        return result;
    }

    int32_t OpenGLShader::GetUniformLocation(const std::string& name) const
    {
        int32_t result = glGetUniformLocation(m_RendererID, name.c_str());
        return result;
    }

    void OpenGLShader::SetUniform(const std::string& fullname, float value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniform1f(GetUniformLocation(fullname), value);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, uint32_t value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniform1ui(GetUniformLocation(fullname), value);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, int value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniform1i(GetUniformLocation(fullname), value);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::vec2& value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniform2fv(GetUniformLocation(fullname), 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::ivec2& value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniform2i(GetUniformLocation(fullname), value.x, value.y);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::ivec3& value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniform3i(GetUniformLocation(fullname), value.x, value.y, value.z);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::ivec4& value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniform4i(GetUniformLocation(fullname), value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::vec3& value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniform3fv(GetUniformLocation(fullname), 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::vec4& value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniform4fv(GetUniformLocation(fullname), 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::mat3& value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniformMatrix3fv(GetUniformLocation(fullname), 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::SetUniform(const std::string& fullname, const glm::mat4& value)
    {
        //assert(m_UniformLocations.find(fullname) != m_UniformLocations.end());
        //GLint location = m_UniformLocations.at(fullname);
        glUniformMatrix4fv(GetUniformLocation(fullname), 1, GL_FALSE, glm::value_ptr(value));
    }

    const ShaderResourceDeclaration* OpenGLShader::GetShaderResource(const std::string& name)
    {
        if (m_Resources.find(name) == m_Resources.end()) return nullptr;

        return &m_Resources.at(name);
    }

    void OpenGLShader::UploadUniformInt(uint32_t location, int32_t value) { glUniform1i(location, value); }

    void OpenGLShader::UploadUniformIntArray(uint32_t location, int32_t* values, int32_t count)
    {
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadUniformFloat(uint32_t location, float value) { glUniform1f(location, value); }

    void OpenGLShader::UploadUniformFloat2(uint32_t location, const glm::vec2& value)
    {
        glUniform2f(location, value.x, value.y);
    }

    void OpenGLShader::UploadUniformFloat3(uint32_t location, const glm::vec3& value)
    {
        glUniform3f(location, value.x, value.y, value.z);
    }

    void OpenGLShader::UploadUniformFloat4(uint32_t location, const glm::vec4& value)
    {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    void OpenGLShader::UploadUniformMat3(uint32_t location, const glm::mat3& value)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::UploadUniformMat4(uint32_t location, const glm::mat4& value)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }

    void OpenGLShader::UploadUniformMat4Array(uint32_t location, const glm::mat4& values, uint32_t count)
    {
        glUniformMatrix4fv(location, count, GL_FALSE, glm::value_ptr(values));
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int32_t value)
    {
        int32_t location = GetUniformLocation(name);
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformUInt(const std::string& name, uint32_t value)
    {
        int32_t location = GetUniformLocation(name);
        glUniform1ui(location, value);
    }

    void OpenGLShader::UploadUniformIntArray(const std::string& name, int32_t* values, uint32_t count)
    {
        int32_t location = GetUniformLocation(name);
        glUniform1iv(location, count, values);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        glUseProgram(m_RendererID);
        auto location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1) glUniform1f(location, value);
        else
            std::cout << "Uniform '" << name << "' not found!\n";
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
    {
        glUseProgram(m_RendererID);
        auto location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1) glUniform2f(location, values.x, values.y);
        else
            std::cout << "Uniform '" << name << "' not found!\n";
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
    {
        glUseProgram(m_RendererID);
        auto location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1) glUniform3f(location, values.x, values.y, values.z);
        else
            std::cout << "Uniform '" << name << "' not found!\n";
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
    {
        glUseProgram(m_RendererID);
        auto location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1) glUniform4f(location, values.x, values.y, values.z, values.w);
        else
            std::cout << "Uniform '" << name << "' not found!\n";
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& values)
    {
        glUseProgram(m_RendererID);
        auto location = glGetUniformLocation(m_RendererID, name.c_str());
        if (location != -1) glUniformMatrix4fv(location, 1, GL_FALSE, (const float*) &values);
        else
            std::cout << "Uniform '" << name << "' not found!\n";
    }

    void OpenGLShader::AddResource(std::string_view name, ShaderResourceDeclaration resource)
    {
        m_Resources.emplace(name, resource);
    }

    void OpenGLShader::Destroy() {}
}// namespace Engine
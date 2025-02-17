#include <Util/File.hpp>
#include <Core/Allocator.hpp>
#include <Core/STL/CFilesystem.h>
#include <Renderer/Shared/ShaderUniform.h>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Renderer/Predefines.hpp>

#include <string>
#include <sstream>
#include <limits>
#include <fstream>
#include <filesystem>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Engine
{


    void Compile(ShaderData* shaderData, const std::string& vertexData, const std::string& fragmentData);

    void CheckShader(ShaderData* shaderData, GLuint id, GLuint type, GLint* ret, const char* onfail);

    std::string ReadShaderFromFile(ShaderData* shaderData, const std::string& filepath, ShaderType shaderType);

    std::unordered_map<GLenum, std::string>* PreProcess(ShaderData* shaderData, const std::string& source);

    int32_t GetUniformLocation(ShaderData* shaderData, const char* name);

    EXPORT_RENDERER void ShaderReload(void* data, int recompile)
    {
        ShaderData* m_Data = (ShaderData*) data;
        m_Data->shaderSource =
                (void*) PreProcess(m_Data, std::filesystem::path(m_Data->assetPath).append(m_Data->name).string());

        Compile(m_Data, asTPtr(m_Data->shaderSource, std::unordered_map<GLenum, std::string>)->at(GL_VERTEX_SHADER),
                asTPtr(m_Data->shaderSource, std::unordered_map<GLenum, std::string>)->at(GL_FRAGMENT_SHADER));
    }

    EXPORT_RENDERER void ShaderLoad(void** data, const char* path)
    {
        ShaderData* m_Data = (ShaderData*) *data;
        if (m_Data == nullptr) { *data = Engine::Allocator::Allocate<ShaderData>(); }
        m_Data = (ShaderData*) *data;

        m_Data->assetPath = strdup(std::filesystem::absolute(path).parent_path().string().c_str());
        m_Data->name = strdup(std::filesystem::path(path).filename().string().c_str());
        ShaderReload(m_Data, 1);
    }

    void Compile(ShaderData* shaderData, const std::string& vertexData, const std::string& fragmentData)
    {

        // Renderer::SubmitAndFlush(std::function<void(void)>([&]() {
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
        // }));
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

    std::unordered_map<GLenum, std::string>* PreProcess(ShaderData* shaderData, const std::string& source)
    {
        std::unordered_map<GLenum, std::string>* shaderSources =
                Allocator::Allocate<std::unordered_map<GLenum, std::string>>();

        shaderSources->emplace(GL_VERTEX_SHADER, ReadShaderFromFile(shaderData, source, ShaderType::Vertex));
        shaderSources->emplace(GL_FRAGMENT_SHADER, ReadShaderFromFile(shaderData, source, ShaderType::Fragment));

        return shaderSources;
    }

    EXPORT_RENDERER void ShaderBind(void* data)
    {
        ShaderData* m_Data = (ShaderData*) data;
        glUseProgram(m_Data->rendererID);
    }

    EXPORT_RENDERER unsigned int ShaderGetID(void* data) { return asTPtr(data, ShaderData)->rendererID; }

    inline static bool file_read_string(const int8_t* path, size_t* len, std::string* buffer)
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

            if (!FileExists(path.c_str())) { 
                LOG_ERROR("%s Does not exist!\n", path.c_str());
            }

            auto resultStatus = file_read_string((const int8_t*) path.c_str(), &bufferLen, &buffer);
            if (resultStatus != false) { result = buffer; }
        }
        catch (std::ios_base::failure& e)
        {
            LOG_ERROR("%s %s\n", e.what(), filepath.c_str());
        }
        return result;
    }

    EXPORT_RENDERER void ShaderDestroy(void** data)
    {
        ShaderData* m_Data = (ShaderData*) *data;
        if (m_Data)
        {
            glDeleteProgram(m_Data->rendererID);
            if (m_Data->name) { free(m_Data->name); }
            if (m_Data->assetPath) { free(m_Data->assetPath); }
            Allocator::Deallocate(m_Data);
            *data = nullptr;
        }
    }

    int32_t GetUniformLocation(ShaderData* shaderData, const char* name)
    {
        int32_t result = glGetUniformLocation(shaderData->rendererID, name);
        return result;
    }

    EXPORT_RENDERER void ShaderSetUniformF(void* data, const char* fullname, float value)
    {
        glUniform1f(GetUniformLocation((ShaderData*) data, fullname), value);
    }

    EXPORT_RENDERER void ShaderSetUniformI(void* data, const char* fullname, int32_t value)
    {
        glUniform1i(GetUniformLocation((ShaderData*) data, fullname), value);
    }

    EXPORT_RENDERER void ShaderSetUniformUI(void* data, const char* fullname, uint32_t value)
    {
        glUniform1ui(GetUniformLocation((ShaderData*) data, fullname), value);
    }

    EXPORT_RENDERER void ShaderSetUniform2F(void* data, const char* fullname, float x, float y)
    {
        glUniform2f(GetUniformLocation((ShaderData*) data, fullname), x, y);
    }

    EXPORT_RENDERER void ShaderSetUniform3F(void* data, const char* fullname, float x, float y, float z)
    {
        glUniform3f(GetUniformLocation((ShaderData*) data, fullname), x, y, z);
    }

    EXPORT_RENDERER void ShaderSetUniform4F(void* data, const char* fullname, float x, float y, float z, float w)
    {
        glUniform4f(GetUniformLocation((ShaderData*) data, fullname), x, y, z, w);
    }

    EXPORT_RENDERER void ShaderSetUniform2I(void* data, const char* fullname, int32_t x, int32_t y)
    {
        glUniform2i(GetUniformLocation((ShaderData*) data, fullname), x, y);
    }

    EXPORT_RENDERER void ShaderSetUniform3I(void* data, const char* fullname, int32_t x, int32_t y, int32_t z)
    {
        glUniform3i(GetUniformLocation((ShaderData*) data, fullname), x, y, z);
    }

    EXPORT_RENDERER void ShaderSetUniform4I(void* data, const char* fullname, int32_t x, int32_t y, int32_t z,
                                            int32_t w)
    {
        glUniform4i(GetUniformLocation((ShaderData*) data, fullname), x, y, z, w);
    }

    EXPORT_RENDERER void ShaderSetUniformMat3(void* data, const char* fullname, void* matrix)
    {
        glUniformMatrix3fv(GetUniformLocation((ShaderData*) data, fullname), 1, GL_FALSE,
                           glm::value_ptr(*(glm::mat3*) matrix));
    }

    EXPORT_RENDERER void ShaderSetUniformMat4(void* data, const char* fullname, void* matrix)
    {
        glUniformMatrix4fv(GetUniformLocation((ShaderData*) data, fullname), 1, GL_FALSE,
                           glm::value_ptr(*(glm::mat4*) matrix));
    }

    EXPORT_RENDERER const char* ShaderGetName(void* data) { return ((ShaderData*) data)->name; }

    EXPORT_RENDERER const char* ShaderGetPath(void* data) { return ((ShaderData*) data)->assetPath; };
}// namespace Engine
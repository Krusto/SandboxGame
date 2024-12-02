#include <Core/Log.hpp>
#include <GLSLReflection.hpp>
#include <filesystem>
#include <cstdint>
#include <string>
#include <fstream>

static bool file_read_string(const char* path, size_t* len, std::string* buffer)
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

int main()
{
    std::string source;
    size_t len;

    std::string path = "E:/Projects/SandboxGame/ShaderReflectionTest/Test.glsl";

    if (file_read_string(path.c_str(), &len, &source))
    {
        Engine::GLSLReflection reflection;
        reflection.Init(source);

        auto uniformBlocksLayout = reflection.GetUniformBlocksLayout();

        for (auto& uniformBlockLayout: uniformBlocksLayout)
        {

            LOG_INFO("Uniform block: %s\n", uniformBlockLayout.Name.c_str());
            LOG_INFO("    Binding: %d\n", uniformBlockLayout.Binding);
            LOG_INFO("    Layout: %s\n",
                     Engine::ShaderDataType::ShaderBlockMemoryLayoutString(uniformBlockLayout.Layout).c_str());

            LOG_INFO("    Uniforms:\n");
            for (auto& uniform: uniformBlockLayout.Uniforms)
            {
                LOG_INFO("        %s: %s (Offset %d)\n",
                         Engine::ShaderDataType::ShaderDataTypeString(uniform.Type).c_str(), uniform.Name.c_str(),
                         uniform.Offset);
            }
        }
    }
    else { LOG_ERROR("File %s does not exist!\n", path.c_str()); }
    return 0;
}
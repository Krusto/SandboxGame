#include "Shader.hpp"
#include <Renderer/OpenGL/OpenGLShader.hpp>
#include <Renderer/RendererAPI.hpp>
#include <Core/Allocator.hpp>

namespace Engine
{

    Shader* Shader::Load(const std::string& path)
    {
        Shader* ptr = Engine::Allocator::Allocate<OpenGLShader>(path, false);
        return ptr;
    }
}// namespace Engine
#include "Shader.hpp"
#include <Renderer/OpenGL/OpenGLShader.hpp>
#include <Renderer/RendererAPI.hpp>

namespace Engine
{

    Shader* Shader::Load(const std::string& path) { return new OpenGLShader(path, false); }
}// namespace Engine
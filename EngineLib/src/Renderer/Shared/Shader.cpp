#include "Shader.hpp"

namespace Engine
{

    Shader Shader::Create(const std::string& path)
    {
        Shader ptr;
        ptr.Load(path);
        return ptr;
    }

    RendererCommand Shader::BindCommand() const
    {
        return RendererCommand([=]() { Bind(); });
    }
}// namespace Engine
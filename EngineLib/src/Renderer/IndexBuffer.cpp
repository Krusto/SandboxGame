#include "IndexBuffer.hpp"
#include <Renderer/OpenGL/OpenGLIndexBuffer.hpp>
#include <Renderer/RendererAPI.hpp>

namespace Engine
{

    IndexBuffer* IndexBuffer::Create(const uint32_t* data, uint32_t length)
    {
        auto ptr = new OpenGLIndexBuffer();
        ptr->Init(data, length);
        return ptr;
    }
}// namespace Engine
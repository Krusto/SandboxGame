#include "IndexBuffer.hpp"
#include <Renderer/OpenGL/OpenGLIndexBuffer.hpp>
#include <Renderer/RendererAPI.hpp>
#include <Core/Allocator.hpp>

namespace Engine
{

    IndexBuffer* IndexBuffer::Create(const uint32_t* data, uint32_t length)
    {
        Allocate(OpenGLIndexBuffer, ptr);
        ptr->Init(data, length);
        return ptr;
    }
}// namespace Engine
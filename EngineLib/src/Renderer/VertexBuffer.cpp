#include "VertexBuffer.hpp"
#include <Renderer/OpenGL/OpenGLVertexBuffer.hpp>
#include <Renderer/RendererAPI.hpp>
#include <Core/Allocator.hpp>

namespace Engine
{

    VertexBuffer* VertexBuffer::Create(const VertexLayout& layout, float* data, uint32_t length)
    {
        VertexBuffer* ptr;
        Allocate(OpenGLVertexBuffer, ptr);
        ptr->Init(layout, data, length);
        return ptr;
    }

}// namespace Engine
#include "VertexBuffer.hpp"
#include <Renderer/OpenGL/OpenGLVertexBuffer.hpp>
#include <Renderer/RendererAPI.hpp>

namespace Engine
{

    VertexBuffer* VertexBuffer::Create(const VertexLayout& layout, float* data, uint32_t length)
    {
        auto ptr = new OpenGLVertexBuffer();
        ptr->Init(layout, data, length);
        return ptr;
    }

}// namespace Engine
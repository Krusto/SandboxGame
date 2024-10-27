#include "VertexBuffer.hpp"
#include <Renderer/OpenGL/OpenGLVertexBuffer.hpp>
#include <Renderer/RendererAPI.hpp>

namespace Engine
{

    VertexBuffer* VertexBuffer::Create(const VertexLayout& layout, float* data, uint32_t length)
    {
        return new OpenGLVertexBuffer(layout, data, length);
    }

}// namespace Engine
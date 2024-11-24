#include "VertexBuffer.hpp"
#include <Renderer/OpenGL/OpenGLVertexBuffer.hpp>
#include <Renderer/RendererAPI.hpp>
#include <Core/Allocator.hpp>

namespace Engine
{

    VertexBuffer* VertexBuffer::Create(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length)
    {
        VertexBuffer* ptr = Engine::Allocator::Allocate<OpenGLVertexBuffer>();
        ptr->Init(va, layout, data, length);
        return ptr;
    }

}// namespace Engine
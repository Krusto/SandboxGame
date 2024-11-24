#include "IndexBuffer.hpp"
#include <Renderer/OpenGL/OpenGLIndexBuffer.hpp>
#include <Renderer/RendererAPI.hpp>
#include <Core/Allocator.hpp>

namespace Engine
{

    IndexBuffer* IndexBuffer::Create(VertexArray* va, const uint32_t* data, uint32_t length)
    {
        IndexBuffer* ptr = Engine::Allocator::Allocate<OpenGLIndexBuffer>();
        ptr->Init(va, data, length);
        return ptr;
    }
}// namespace Engine
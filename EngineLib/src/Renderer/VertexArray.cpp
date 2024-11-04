#include "VertexArray.hpp"
#include <Renderer/OpenGL/OpenGLVertexArray.hpp>
#include <Renderer/RendererAPI.hpp>
#include <Core/Allocator.hpp>

namespace Engine
{

    VertexArray* VertexArray::Create(uint32_t indexCount)
    {

        auto ptr = Allocator::Allocate<OpenGLVertexArray>();
        ptr->Init(indexCount);
        return ptr;
    }

    void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
    {
        Bind();
        m_VertexBuffer = vertexBuffer;
    }

    void VertexArray::AddIndexBuffer(IndexBuffer* indexBuffer) { m_IndexBuffer = indexBuffer; }
}// namespace Engine
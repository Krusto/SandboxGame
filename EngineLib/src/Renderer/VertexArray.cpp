#include "VertexArray.hpp"
#include <Renderer/OpenGL/OpenGLVertexArray.hpp>
#include <Renderer/RendererAPI.hpp>

namespace Engine
{

    VertexArray* VertexArray::Create(uint32_t indexCount) { return new OpenGLVertexArray(indexCount); }

    void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
    {
        Bind();
        m_VertexBuffer = vertexBuffer;
    }

    void VertexArray::AddIndexBuffer(IndexBuffer* indexBuffer) { m_IndexBuffer = indexBuffer; }
}// namespace Engine
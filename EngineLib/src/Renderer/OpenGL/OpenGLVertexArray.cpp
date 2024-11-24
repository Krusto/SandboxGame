#include <glad/glad.h>
#include <cassert>
#include "OpenGLVertexArray.hpp"
#include <Renderer/VertexBuffer.hpp>
#include <Renderer/IndexBuffer.hpp>

namespace Engine
{

    void OpenGLVertexArray::Init(uint32_t indexCount)
    {
        glCreateVertexArrays(1, &m_ID);
        this->IndexCount = indexCount;
    }

    void OpenGLVertexArray::Bind() const
    {
        assert(m_ID != 0);
        glBindVertexArray(m_ID);
    }

    void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

    void OpenGLVertexArray::Destroy()
    {
        if (m_ID != 0)
        {
            glDeleteVertexArrays(1, &m_ID);
            if (m_VertexBuffer)
            {
                m_VertexBuffer->Destroy();
                Engine::Allocator::Deallocate(m_VertexBuffer);
            }
            if (m_IndexBuffer)
            {
                m_IndexBuffer->Destroy();
                Engine::Allocator::Deallocate(m_IndexBuffer);
            }
        }
    }

    void OpenGLVertexArray::AddVertexBuffer(const VertexLayout& layout, float* data, uint32_t length)
    {
        Bind();
        m_VertexBuffer = VertexBuffer::Create(this, layout, data, length);
        glVertexArrayVertexBuffer(m_ID, 0, m_VertexBuffer->GetID(), 0, layout.stride);
    }

    void OpenGLVertexArray::AddIndexBuffer(const uint32_t* data, uint32_t length)
    {
        Bind();
        m_IndexBuffer = IndexBuffer::Create(this, data, length);
        glVertexArrayElementBuffer(m_ID, m_IndexBuffer->GetID());
    }

}// namespace Engine
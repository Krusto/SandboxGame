#include <glad/glad.h>
#include <cassert>
#include "OpenGLVertexArray.hpp"

namespace Engine
{

    OpenGLVertexArray::OpenGLVertexArray(uint32_t indexCount)
    {
        glGenVertexArrays(1, &m_ID);
        this->IndexCount = indexCount;
    }

    void OpenGLVertexArray::Bind() const
    {
        assert(m_ID != 0);
        glBindVertexArray(m_ID);
    }

    void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        if (m_ID != 0)
        {
            glDeleteVertexArrays(1, &m_ID);
            uint32_t vbo = m_VertexBuffer->GetID(), ibo = m_IndexBuffer->GetID();
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ibo);
        }
    }
}// namespace Engine
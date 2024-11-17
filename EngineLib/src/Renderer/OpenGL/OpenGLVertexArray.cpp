#include <glad/glad.h>
#include <cassert>
#include "OpenGLVertexArray.hpp"

namespace Engine
{

    void OpenGLVertexArray::Init(uint32_t indexCount)
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

    void OpenGLVertexArray::Destroy()
    {
        if (m_ID != 0)
        {
            glDeleteVertexArrays(1, &m_ID);
            if (m_VertexBuffer)
            {
                m_VertexBuffer->Destroy();
                Deallocate(m_VertexBuffer);
            }
            if (m_IndexBuffer)
            {
                m_IndexBuffer->Destroy();
                Deallocate(m_IndexBuffer);
            }
        }
    }

}// namespace Engine
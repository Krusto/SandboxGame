
#include <glad/glad.h>
#include <cassert>
#include "OpenGLIndexBuffer.hpp"

namespace Engine
{

    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, uint32_t length)
    {
        glGenBuffers(1, &m_ID);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * length, data, GL_STATIC_DRAW);

        indexCount = length;
    }

    void OpenGLIndexBuffer::Bind() const
    {
        assert(m_ID != 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    OpenGLIndexBuffer OpenGLIndexBuffer::Create(const uint32_t* data, uint32_t length)
    {
        return OpenGLIndexBuffer{data, length};
    }
}// namespace Engine


#include <glad/glad.h>
#include <cassert>
#include "OpenGLIndexBuffer.hpp"

namespace Engine
{


    void OpenGLIndexBuffer::Init(VertexArray* va, const uint32_t* data, uint32_t length)
    {
        glCreateBuffers(1, &m_ID);

        glNamedBufferStorage(m_ID, sizeof(uint32_t) * length, data, GL_DYNAMIC_STORAGE_BIT);

        indexCount = length;
    }

    void OpenGLIndexBuffer::Destroy()
    {
        if (m_ID != 0) { glDeleteBuffers(1, &m_ID); }
    }

    void OpenGLIndexBuffer::Bind() const
    {
        assert(m_ID != 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

}// namespace Engine

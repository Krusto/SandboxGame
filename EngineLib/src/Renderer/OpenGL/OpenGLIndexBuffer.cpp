
#include <Renderer/OpenGL/OpenGLIndexBuffer.hpp>
#include <Renderer/IndexBuffer.hpp>
#include <Core/Allocator.hpp>

#include <cassert>
#include <glad/glad.h>

namespace Engine
{

    void IndexBuffer::Init(VertexArray* va, const uint32_t* data, uint32_t length)
    {
        m_Data = Allocator::Allocate<IndexBufferData>();
        glCreateBuffers(1, &m_Data->m_ID);

        glNamedBufferStorage(m_Data->m_ID, sizeof(uint32_t) * length, data, GL_DYNAMIC_STORAGE_BIT);

        indexCount = length;
    }

    void IndexBuffer::Destroy()
    {
        if (m_Data == nullptr) { return; }
        if (m_Data->m_ID != 0) { glDeleteBuffers(1, &m_Data->m_ID); }
        Allocator::Deallocate(m_Data);
        m_Data = nullptr;
    }

    void IndexBuffer::Bind() const
    {
        assert(m_Data->m_ID != 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Data->m_ID);
    }

    uint32_t IndexBuffer::GetID() const { return m_Data->m_ID; }

}// namespace Engine

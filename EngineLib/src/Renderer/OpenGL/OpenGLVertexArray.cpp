#include <glad/glad.h>
#include <cassert>
#include "Renderer/Shared/VertexArray.hpp"
#include <Renderer/Shared/VertexBuffer.hpp>
#include <Renderer/Shared/IndexBuffer.hpp>

namespace Engine
{
    struct VertexArrayData {
        uint32_t indexCount{};
        VertexBuffer vertexBuffer{};
        IndexBuffer indexBuffer{};
        uint32_t id{};
    };

    void VertexArray::Init(uint32_t indexCount)
    {
        if (m_Data == nullptr) { m_Data = Engine::Allocator::Allocate<VertexArrayData>(); }
        glCreateVertexArrays(1, &m_Data->id);
        m_Data->indexCount = indexCount;
    }

    void VertexArray::Bind() const
    {
        assert(m_Data->id != 0);
        glBindVertexArray(m_Data->id);
    }

    void VertexArray::Unbind() const { glBindVertexArray(0); }

    void VertexArray::Destroy()
    {
        if (m_Data)
        {
            if (m_Data->id != 0)
            {
                glDeleteVertexArrays(1, &m_Data->id);
                m_Data->vertexBuffer.Destroy();
                m_Data->indexBuffer.Destroy();
            }
            Allocator::Deallocate(m_Data);
            m_Data = nullptr;
        }
    }

    void VertexArray::AddVertexBuffer(const VertexLayout& layout, float* data, uint32_t length)
    {
        Bind();
        m_Data->vertexBuffer = VertexBuffer::Create(this, layout, data, length);
        glVertexArrayVertexBuffer(m_Data->id, 0, m_Data->vertexBuffer.GetID(), 0, layout.stride);
    }

    void VertexArray::AddIndexBuffer(const uint32_t* data, uint32_t length)
    {
        Bind();
        m_Data->indexBuffer = IndexBuffer::Create(this, data, length);
        glVertexArrayElementBuffer(m_Data->id, m_Data->indexBuffer.GetID());
    }

    uint32_t VertexArray::IndexCount() const { return m_Data->indexCount; }

    uint32_t VertexArray::id() const { return m_Data->id; }
}// namespace Engine
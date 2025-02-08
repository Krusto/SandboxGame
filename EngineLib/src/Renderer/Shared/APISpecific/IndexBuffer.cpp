#include "IndexBuffer.hpp"
#include <Core/Allocator.hpp>
#include <Renderer/Renderer.hpp>

namespace Engine
{

    IndexBuffer IndexBuffer::Create(VertexArray* va, const uint32_t* data, uint32_t length)
    {
        IndexBuffer ptr;
        ptr.Init(va, data, length);
        return ptr;
    }

    void IndexBuffer::Init(VertexArray* va, const uint32_t* data, uint32_t length)
    {
        Renderer::GetInstance()->IndexBufferInit((void**) &m_Data, (void*) va, (unsigned int*) data, length);
    }

    void IndexBuffer::Destroy() { Renderer::GetInstance()->IndexBufferDestroy((void**) &m_Data); }

    void IndexBuffer::Bind() const { Renderer::GetInstance()->IndexBufferBind((void*) m_Data); }

    uint32_t IndexBuffer::GetID() const { return Renderer::GetInstance()->IndexBufferGetID((void*) m_Data); }

    uint32_t IndexBuffer::GetSize() const { return Renderer::GetInstance()->IndexBufferGetSize((void*) m_Data); }

    uint32_t IndexBuffer::GetLength() const { return Renderer::GetInstance()->IndexBufferGetLength((void*) m_Data); }
}// namespace Engine
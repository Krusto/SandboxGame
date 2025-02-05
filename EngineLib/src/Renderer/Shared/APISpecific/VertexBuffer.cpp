#include "VertexBuffer.hpp"
#include <Renderer/Shared/function_pointers.h>

namespace Engine
{

    VertexBuffer VertexBuffer::Create(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length)
    {
        VertexBuffer ptr;
        ptr.Init(va, layout, data, length);
        return ptr;
    }

    void VertexBuffer::Init(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length)
    {
        VertexBufferInit((void**) &m_Data, va, (void*) &layout, data, length);
    }

    void VertexBuffer::Bind() const { VertexBufferBind(m_Data); }

    size_t VertexBuffer::GetSize() const { return VertexBufferGetSize(m_Data); }

    void VertexBuffer::Destroy() { VertexBufferDestroy((void**) &m_Data); }

    uint32_t VertexBuffer::GetID() const { return VertexBufferGetID(m_Data); }

}// namespace Engine
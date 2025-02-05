#include "IndexBuffer.hpp"
#include <Renderer/Shared/function_pointers.h>
#include <Core/Allocator.hpp>

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
        IndexBufferInit((void**) &m_Data, (void*) va, (unsigned int*) data, length);
    }

    void IndexBuffer::Destroy() { IndexBufferDestroy((void**) &m_Data); }

    void IndexBuffer::Bind() const { IndexBufferBind((void*) m_Data); }

    uint32_t IndexBuffer::GetID() const { return IndexBufferGetID((void*) m_Data); }

    uint32_t IndexBuffer::GetSize() const { return IndexBufferGetSize((void*) m_Data); }

    uint32_t IndexBuffer::GetLength() const { return IndexBufferGetLength((void*) m_Data); }
}// namespace Engine
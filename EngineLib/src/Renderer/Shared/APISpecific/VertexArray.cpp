#include "VertexArray.hpp"
#include <Renderer/Shared/function_pointers.h>

namespace Engine
{

    VertexArray VertexArray::Create(uint32_t indexCount)
    {
        VertexArray ptr;
        ptr.Init(indexCount);
        return ptr;
    }

    void VertexArray::Init(uint32_t indexCount) { VertexArrayInit((void**) &m_Data, indexCount); }

    void VertexArray::Bind() const { VertexArrayBind((void*) m_Data); }

    void VertexArray::Unbind() const { VertexArrayUnbind((void*) m_Data); }

    void VertexArray::Destroy() { VertexArrayDestroy((void**) m_Data); }

    void VertexArray::AddVertexBuffer(const void* layout, float* data, uint32_t length)
    {
        VertexArrayAddVertexBuffer((void*) m_Data, (void*) layout, data, length);
    }

    void VertexArray::AddIndexBuffer(const uint32_t* data, uint32_t length)
    {
        VertexArrayAddIndexBuffer((void*) m_Data, (unsigned int*) data, length);
    }

    uint32_t VertexArray::IndexCount() const { return VertexArrayGetIndexCount((void*) m_Data); }

    uint32_t VertexArray::id() const { return VertexArrayGetID((void*) m_Data); }

}// namespace Engine
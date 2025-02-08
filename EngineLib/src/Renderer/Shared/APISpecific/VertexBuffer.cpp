#include "VertexBuffer.hpp"
#include <Renderer/Renderer.hpp>

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
        Renderer::GetInstance()->VertexBufferInit((void**) &m_Data, va, (void*) &layout, data, length);
    }

    void VertexBuffer::Bind() const { Renderer::GetInstance()->VertexBufferBind(m_Data); }

    size_t VertexBuffer::GetSize() const { return Renderer::GetInstance()->VertexBufferGetSize(m_Data); }

    void VertexBuffer::Destroy() { Renderer::GetInstance()->VertexBufferDestroy((void**) &m_Data); }

    uint32_t VertexBuffer::GetID() const { return Renderer::GetInstance()->VertexBufferGetID(m_Data); }

}// namespace Engine
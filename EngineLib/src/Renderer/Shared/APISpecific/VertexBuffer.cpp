#include "VertexBuffer.hpp"
#include <Renderer/Renderer.hpp>

namespace Engine
{
    VertexBuffer::VertexBuffer(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length)
    {
        Init(va, layout, data, length);
    }

    VertexBuffer VertexBuffer::Create(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length)
    {
        return VertexBuffer(va, layout, data, length);
    }

    void VertexBuffer::Init(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length)
    {
        m_Data = Renderer::GetInstance()->VertexBufferInit(va, (void*) &layout, data, length);
    }

    void VertexBuffer::Bind() const { Renderer::GetInstance()->VertexBufferBind(m_Data); }

    size_t VertexBuffer::GetSize() const { return Renderer::GetInstance()->VertexBufferGetSize(m_Data); }

    void VertexBuffer::Destroy() { Renderer::GetInstance()->VertexBufferDestroy((void**) &m_Data); }

    uint32_t VertexBuffer::GetID() const { return Renderer::GetInstance()->VertexBufferGetID(m_Data); }

}// namespace Engine
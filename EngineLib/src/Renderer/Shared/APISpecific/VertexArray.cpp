#include "VertexArray.hpp"
#include <Renderer/Renderer.hpp>

namespace Engine
{
    VertexArray::VertexArray(uint32_t indexCount) { Init(indexCount); }

    VertexArray VertexArray::Create(uint32_t indexCount) { return VertexArray(indexCount); }

    void VertexArray::Init(uint32_t indexCount) { m_Data = Renderer::GetInstance()->VertexArrayInit(indexCount); }

    void VertexArray::Bind() const { Renderer::GetInstance()->VertexArrayBind((void*) m_Data); }

    void VertexArray::Unbind() const { Renderer::GetInstance()->VertexArrayUnbind((void*) m_Data); }

    void VertexArray::Destroy() { Renderer::GetInstance()->VertexArrayDestroy((void**) &m_Data); }

    void VertexArray::AddVertexBuffer(const void* layout, float* data, uint32_t length)
    {
        Renderer::GetInstance()->VertexArrayAddVertexBuffer((void*) m_Data, (void*) layout, data, length);
    }

    void VertexArray::AddIndexBuffer(const uint32_t* data, uint32_t length)
    {
        Renderer::GetInstance()->VertexArrayAddIndexBuffer((void*) m_Data, (unsigned int*) data, length);
    }

    uint32_t VertexArray::IndexCount() const
    {
        return Renderer::GetInstance()->VertexArrayGetIndexCount((void*) m_Data);
    }

    uint32_t VertexArray::id() const { return Renderer::GetInstance()->VertexArrayGetID((void*) m_Data); }

}// namespace Engine
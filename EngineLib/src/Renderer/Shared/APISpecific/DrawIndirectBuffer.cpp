#include "DrawIndirectBuffer.hpp"
#include <Renderer/Renderer.hpp>

namespace Engine
{
    DrawIndirectBuffer::DrawIndirectBuffer(uint8_t* data, std::size_t size, StorageBufferType type)
    {
        Init(data, size, type);
    }

    DrawIndirectBuffer DrawIndirectBuffer::Create(uint8_t* data, std::size_t size, StorageBufferType type)
    {
        return DrawIndirectBuffer(data, size, type);
    }

    void DrawIndirectBuffer::Init(uint8_t* data, std::size_t size, StorageBufferType type)
    {
        Renderer::GetInstance()->DrawIndirectBufferInit(&m_Data, (int8_t*) data, size, type);
    }

    void DrawIndirectBuffer::Bind() const { Renderer::GetInstance()->DrawIndirectBufferBind(m_Data, 0); }

    void DrawIndirectBuffer::Upload(uint8_t* data, std::size_t size, std::size_t offset) const
    {
        Renderer::GetInstance()->DrawIndirectBufferUpload(m_Data, (int8_t*) data, size, offset);
    }

    void DrawIndirectBuffer::Draw(size_t count, std::size_t stride, std::size_t offset) const
    {
        Renderer::GetInstance()->DrawIndirectBufferDraw(m_Data, count, stride, offset);
    }

    void DrawIndirectBuffer::Unbind() const { Renderer::GetInstance()->DrawIndirectBufferUnbind(m_Data); }

    void DrawIndirectBuffer::Destroy() { Renderer::GetInstance()->DrawIndirectBufferDestroy(&m_Data); }

    uint32_t DrawIndirectBuffer::id() const { return Renderer::GetInstance()->DrawIndirectBufferGetID(m_Data); }


}// namespace Engine
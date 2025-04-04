#include "StorageBuffer.hpp"
#include <Renderer/Renderer.hpp>

namespace Engine
{
    StorageBuffer::StorageBuffer(uint8_t* data, std::size_t size, StorageBufferType type) { Init(data, size, type); }

    StorageBuffer StorageBuffer::Create(uint8_t* data, std::size_t size, StorageBufferType type)
    {
        return StorageBuffer(data, size, type);
    }

    void StorageBuffer::Init(uint8_t* data, std::size_t size, StorageBufferType type)
    {
        Renderer::GetInstance()->StorageBufferInit((void**) &m_Data, nullptr, (char*) data, size, (int) type);
    }

    void StorageBuffer::Upload(uint8_t* data, std::size_t size, std::size_t offset) const
    {
        Renderer::GetInstance()->StorageBufferUpload((void*) m_Data, (char*) data, size, offset);
    }

    void StorageBuffer::Bind(size_t location) const
    {
        Renderer::GetInstance()->StorageBufferBind((void*) m_Data, location);
    }

    void StorageBuffer::Unbind() const { Renderer::GetInstance()->StorageBufferUnbind((void*) m_Data); }

    void StorageBuffer::Destroy() { Renderer::GetInstance()->StorageBufferDestroy((void**) &m_Data); }

    uint32_t StorageBuffer::id() const { return Renderer::GetInstance()->StorageBufferGetID((void*) m_Data); }


}// namespace Engine
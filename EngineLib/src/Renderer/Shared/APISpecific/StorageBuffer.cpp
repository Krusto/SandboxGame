#include "StorageBuffer.hpp"
#include <Renderer/Shared/function_pointers.h>

namespace Engine
{

    StorageBuffer StorageBuffer::Create(uint8_t* data, size_t size, StorageBufferType type)
    {
        StorageBuffer ptr;
        ptr.Init(data, size, type);
        return ptr;
    }

    void StorageBuffer::Init(uint8_t* data, size_t size, StorageBufferType type)
    {
        StorageBufferInit((void**) &m_Data, nullptr, (char*) data, size, (int) type);
    }

    void StorageBuffer::Bind(size_t location) const { StorageBufferBind((void*) m_Data, location); }

    void StorageBuffer::Unbind() const { StorageBufferUnbind((void*) m_Data); }

    void StorageBuffer::Destroy() { StorageBufferDestroy((void**) &m_Data); }

    uint32_t StorageBuffer::id() const { return StorageBufferGetID((void*) m_Data); }


}// namespace Engine
#include "StorageBuffer.hpp"

namespace Engine
{

    StorageBuffer StorageBuffer::Create(uint8_t* data, size_t size, StorageBufferType type)
    {
        StorageBuffer ptr;
        ptr.Init(data, size, type);
        return ptr;
    }


}// namespace Engine
#include "StorageBuffer.hpp"
#include <Renderer/OpenGL/OpenGLStorageBuffer.hpp>
#include <Renderer/RendererAPI.hpp>
#include <Core/Allocator.hpp>

namespace Engine
{

    StorageBuffer* StorageBuffer::Create(uint8_t* data, size_t size, StorageBufferType type)
    {
        StorageBuffer* ptr;
        Allocate(OpenGLStorageBuffer, ptr);
        ptr->Init(data, size, type);
        return ptr;
    }


}// namespace Engine
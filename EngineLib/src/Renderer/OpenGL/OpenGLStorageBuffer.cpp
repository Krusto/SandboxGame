#include <glad/glad.h>
#include <cassert>
#include <Core/Allocator.hpp>
#include <Renderer/Shared/StorageBuffer.hpp>

namespace Engine
{
    struct StorageBufferData {
        uint32_t id;
        StorageBufferType type;
    };

    void StorageBuffer::Init(uint8_t* data, size_t size, StorageBufferType type)
    {
        if (m_Data == nullptr) { m_Data = Engine::Allocator::Allocate<StorageBufferData>(); }
        GLenum usage{};
        switch (type)
        {
            case StorageBufferType::DynamicStorage:
                usage = GL_DYNAMIC_STORAGE_BIT;
                break;
            case StorageBufferType::MapReadOnly:
                usage = GL_MAP_READ_BIT;
                break;
            case StorageBufferType::MapWriteOnly:
                usage = GL_MAP_WRITE_BIT;
                break;
            case StorageBufferType::MapPersistent:
                usage = GL_MAP_PERSISTENT_BIT;
                break;
            case StorageBufferType::MapCoherent:
                usage = GL_MAP_COHERENT_BIT;
                break;
            case StorageBufferType::ClientStorage:
                usage = GL_CLIENT_STORAGE_BIT;
                break;
            default:
                break;
        }

        glGenBuffers(1, &m_Data->id);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_Data->id);
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void StorageBuffer::Bind(size_t location) const
    {
        assert(m_Data->id != 0);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, m_Data->id);
    }

    void StorageBuffer::Unbind() const { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0); }

    void StorageBuffer::Destroy()
    {
        if (m_Data)
        {
            glDeleteBuffers(1, &m_Data->id);
            Allocator::Deallocate(m_Data);
            m_Data = nullptr;
        }
    }

    uint32_t StorageBuffer::id() const { return m_Data->id; }

}// namespace Engine
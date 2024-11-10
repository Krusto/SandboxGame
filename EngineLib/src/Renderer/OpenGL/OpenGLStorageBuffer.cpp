#include <glad/glad.h>
#include <cassert>
#include "OpenGLStorageBuffer.hpp"

namespace Engine
{


    void OpenGLStorageBuffer::Init(uint8_t* data, size_t size, StorageBufferType type)
    {
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

        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ID);
        glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void OpenGLStorageBuffer::Bind() const
    {
        assert(m_ID != 0);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_ID);
    }

    void OpenGLStorageBuffer::Unbind() const { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0); }

    void OpenGLStorageBuffer::Destroy()
    {
        if (m_ID != 0) { glDeleteBuffers(1, &m_ID); }
    }

}// namespace Engine
#include <glad/glad.h>
#include <cassert>
#include <Core/Allocator.hpp>
#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>

namespace Engine
{
    inline static GLenum GetGLBufferUsage(StorageBufferType type)
    {
        switch ((StorageBufferType) type)
        {
            case StorageBufferType::DynamicStorage:
                return GL_DYNAMIC_STORAGE_BIT;
            case StorageBufferType::MapReadOnly:
                return GL_MAP_READ_BIT;
            case StorageBufferType::MapWriteOnly:
                return GL_MAP_WRITE_BIT;
            case StorageBufferType::MapPersistent:
                return GL_MAP_PERSISTENT_BIT;
            case StorageBufferType::MapCoherent:
                return GL_MAP_COHERENT_BIT;
            case StorageBufferType::ClientStorage:
                return GL_CLIENT_STORAGE_BIT;
            default:
                return GL_INVALID_ENUM;
        }
    }

    EXPORT_RENDERER void StorageBufferInit(void** data, void* vertexArray, char* storageData, unsigned int size,
                                           int type)
    {
        if (*data == nullptr) { *data = Engine::Allocator::Allocate<StorageBufferData>(); }
        StorageBufferData* m_Data = static_cast<StorageBufferData*>(*data);
        GLenum usage = GetGLBufferUsage((StorageBufferType) type);

        glCreateBuffers(1, &m_Data->id);
        glNamedBufferStorage(m_Data->id, size, storageData, GL_DYNAMIC_STORAGE_BIT);
    }

    EXPORT_RENDERER void StorageBufferUpload(void* data, char* storageData, size_t size, size_t offset)
    {
        StorageBufferData* m_Data = static_cast<StorageBufferData*>(data);
        glNamedBufferSubData(m_Data->id, offset, size, storageData);
    }

    EXPORT_RENDERER void StorageBufferBind(void* data, unsigned int location)
    {
        StorageBufferData* m_Data = static_cast<StorageBufferData*>(data);
        assert(m_Data->id != 0);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, location, m_Data->id);
    }

    EXPORT_RENDERER void StorageBufferUnbind(void* data) { glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0); }

    EXPORT_RENDERER void StorageBufferDestroy(void** data)
    {
        StorageBufferData* m_Data = static_cast<StorageBufferData*>(*data);
        if (m_Data)
        {
            glDeleteBuffers(1, &m_Data->id);
            Allocator::Deallocate(m_Data);
            *data = nullptr;
        }
    }

    EXPORT_RENDERER uint32_t StorageBufferGetID(void* data)
    {
        StorageBufferData* m_Data = static_cast<StorageBufferData*>(data);
        return m_Data->id;
    }

}// namespace Engine
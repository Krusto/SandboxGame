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

    EXPORT_RENDERER void DrawIndirectBufferInit(DrawIndirectBufferData** data, int8_t* storageData, size_t size,
                                                StorageBufferType type)
    {
        if (*data == nullptr) { *data = Engine::Allocator::Allocate<DrawIndirectBufferData>(); }

        DrawIndirectBufferData* m_Data = *data;

        GLenum usage = GetGLBufferUsage((StorageBufferType) type);

        glCreateBuffers(1, &m_Data->id);
        glNamedBufferStorage(m_Data->id, size, storageData, usage);
        if (storageData != nullptr) { return; }

        void* ptr = glMapNamedBuffer(m_Data->id, GL_WRITE_ONLY);
        memset(ptr, 0, size);
        glUnmapNamedBuffer(m_Data->id);
    }

    EXPORT_RENDERER void DrawIndirectBufferUpload(DrawIndirectBufferData* data, int8_t* storageData, size_t size,
                                                  size_t offset)
    {
        glNamedBufferSubData(data->id, offset, size, storageData);
    }

    EXPORT_RENDERER void DrawIndirectBufferDraw(DrawIndirectBufferData* data, size_t count, size_t stride,
                                                size_t offset)
    {
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (void*) offset, count, stride);
    }

    EXPORT_RENDERER void DrawIndirectBufferBind(DrawIndirectBufferData* data, size_t location)
    {
        assert(data->id != 0);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, data->id);
    }

    EXPORT_RENDERER void DrawIndirectBufferUnbind(DrawIndirectBufferData* data)
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    }

    EXPORT_RENDERER void DrawIndirectBufferDestroy(DrawIndirectBufferData** data)
    {
        if (*data)
        {
            glDeleteBuffers(1, &(*data)->id);
            Allocator::Deallocate(*data);
            *data = nullptr;
        }
    }

    EXPORT_RENDERER uint32_t DrawIndirectBufferGetID(DrawIndirectBufferData* data) { return data->id; }


}// namespace Engine
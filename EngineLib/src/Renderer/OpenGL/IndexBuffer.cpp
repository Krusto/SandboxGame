
#include <Core/Allocator.hpp>
#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>

#include <cassert>
#include <glad/glad.h>

namespace Engine
{
    EXPORT_RENDERER void IndexBufferInit(void** data, void* vertexArray, const uint32_t* indexData, uint32_t length)
    {
        if (*data == nullptr) { *data = Allocator::Allocate<IndexBufferData>(); }

        IndexBufferData* m_Data = static_cast<IndexBufferData*>(*data);
        glCreateBuffers(1, &m_Data->id);

        glNamedBufferStorage(m_Data->id, sizeof(uint32_t) * length, data, GL_DYNAMIC_STORAGE_BIT);

        m_Data->stride = sizeof(uint32_t);
        m_Data->count = length;
    }

    EXPORT_RENDERER void IndexBufferDestroy(void** data)
    {
        if (*data == nullptr) { return; }

        IndexBufferData* m_Data = static_cast<IndexBufferData*>(*data);
        if (m_Data->id != 0) { glDeleteBuffers(1, &m_Data->id); }
        Allocator::Deallocate(m_Data);
        *data = nullptr;
    }

    EXPORT_RENDERER void IndexBufferBind(void* data)
    {
        IndexBufferData* m_Data = static_cast<IndexBufferData*>(data);
        assert(m_Data->id != 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Data->id);
    }

    EXPORT_RENDERER uint32_t IndexBufferGetID(void* data) { return static_cast<IndexBufferData*>(data)->id; }

    EXPORT_RENDERER uint32_t IndexBufferGetSize(void* data)
    {
        auto m_Data = static_cast<IndexBufferData*>(data);
        return m_Data->count * m_Data->stride;
    }

    EXPORT_RENDERER uint32_t IndexBufferGetLength(void* data) { return static_cast<IndexBufferData*>(data)->count; }

}// namespace Engine

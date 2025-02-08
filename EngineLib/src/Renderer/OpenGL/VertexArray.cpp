#include <glad/glad.h>
#include <cassert>
#include <Core/Allocator.hpp>
#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Renderer/OpenGL/ExportedFunctions.hpp>
#include <Renderer/Shared/Vertex.hpp>

namespace Engine
{

    EXPORT_RENDERER void VertexArrayInit(void** data, uint32_t indexCount)
    {
        if (*data == nullptr) { *data = Allocator::Allocate<VertexArrayData>(); }

        VertexArrayData* arrayData = (VertexArrayData*)*data;
        glCreateVertexArrays(1, &arrayData->id);
        arrayData->indexCount = indexCount;
    }

    EXPORT_RENDERER void VertexArrayBind(void* data)
    {
        VertexArrayData* m_Data = static_cast<VertexArrayData*>(data);
        assert(m_Data->id != 0);
        glBindVertexArray(m_Data->id);
    }

    EXPORT_RENDERER void VertexArrayUnbind(void* data) { glBindVertexArray(0); }

    EXPORT_RENDERER void VertexArrayDestroy(void** data)
    {
        VertexArrayData* m_Data = static_cast<VertexArrayData*>(*data);
        if (m_Data)
        {
            if (m_Data->id != 0)
            {
                glDeleteVertexArrays(1, &m_Data->id);
                VertexBufferDestroy((void**) &m_Data->vertexBufferData);
                IndexBufferDestroy((void**) &m_Data->indexBufferData);
            }
            Allocator::Deallocate(m_Data);
            m_Data = nullptr;
        }
    }

    EXPORT_RENDERER void VertexArrayAddVertexBuffer(void* data, void* layout, float* vertexData, unsigned int length)
    {
        VertexArrayBind(data);
        VertexArrayData* m_Data = static_cast<VertexArrayData*>(data);
        VertexBufferInit((void**) &m_Data->vertexBufferData, data, layout, vertexData, length);
        glVertexArrayVertexBuffer(m_Data->id, 0, ((VertexBufferData*) (m_Data->vertexBufferData))->id, 0,
                                  ((VertexLayout*) layout)->stride);
    }

    EXPORT_RENDERER void VertexArrayAddIndexBuffer(void* data, unsigned int* indexData, unsigned int length)
    {
        VertexArrayData* m_Data = static_cast<VertexArrayData*>(data);
        VertexArrayBind(data);
        IndexBufferInit((void**) &m_Data->indexBufferData, data, indexData, length);
        glVertexArrayElementBuffer(m_Data->id, m_Data->indexBufferData->id);
    }

    EXPORT_RENDERER uint32_t VertexArrayGetIndexCount(void* data)
    {
        VertexArrayData* m_Data = static_cast<VertexArrayData*>(data);
        return m_Data->indexCount;
    }

    EXPORT_RENDERER uint32_t VertexArrayGetID(void* data)
    {
        VertexArrayData* m_Data = static_cast<VertexArrayData*>(data);
        return m_Data->id;
    }
}// namespace Engine
#include <glad/glad.h>
#include <cassert>
#include <Core/Allocator.hpp>
#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Renderer/OpenGL/ExportedFunctions.hpp>
#include <Renderer/Shared/VertexLayout.hpp>
#ifdef __cplusplus
namespace Engine
{
#endif

    EXPORT_RENDERER VertexArrayData* VertexArrayInit(uint32_t indexCount)
    {
        VertexArrayData* data = Allocator::Allocate<VertexArrayData>();
        glCreateVertexArrays(1, &data->id);
        data->indexCount = indexCount;

        return data;
    }

    EXPORT_RENDERER void VertexArrayBind(VertexArrayData* data)
    {
        assert(data->id != 0);
        glBindVertexArray(data->id);
    }

    EXPORT_RENDERER void VertexArrayUnbind(VertexArrayData* data) { glBindVertexArray(0); }

    EXPORT_RENDERER void VertexArrayDestroy(VertexArrayData** data)
    {
        if (data)
        {
            if ((*data)->id != 0)
            {
                glDeleteVertexArrays(1, &(*data)->id);
                VertexBufferDestroy(&(*data)->vertexBufferData);
                IndexBufferDestroy((void**) &(*data)->indexBufferData);
            }
            Allocator::Deallocate(*data);
            *data = nullptr;
        }
    }

    EXPORT_RENDERER void VertexArrayAddVertexBuffer(VertexArrayData* data, VertexLayout* layout, float* vertexData,
                                                    unsigned int length)
    {
        VertexArrayBind(data);
        data->vertexBufferData = VertexBufferInit( data, layout, vertexData, length);
        glVertexArrayVertexBuffer(data->id, 0, data->vertexBufferData->id, 0, VertexLayoutGetStride(layout));
    }

    EXPORT_RENDERER void VertexArrayAddIndexBuffer(VertexArrayData* data, unsigned int* indexData, unsigned int length)
    {
        VertexArrayBind(data);
        IndexBufferInit((void**) &data->indexBufferData, data, indexData, length);
        glVertexArrayElementBuffer(data->id, data->indexBufferData->id);
    }

    EXPORT_RENDERER uint32_t VertexArrayGetIndexCount(VertexArrayData* data) { return data->indexCount; }

    EXPORT_RENDERER uint32_t VertexArrayGetID(VertexArrayData* data) { return data->id; }
#ifdef __cplusplus
}// namespace Engine
#endif

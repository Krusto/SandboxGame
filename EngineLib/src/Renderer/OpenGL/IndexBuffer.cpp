
#include <Core/Allocator.hpp>
#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>

#include <glad/glad.h>

namespace Engine
{
    EXPORT_RENDERER void IndexBufferInit(IndexBufferData** data, VertexArrayData* vertexArray,
                                         const uint32_t* indexData, uint32_t length)
    {
        if (*data == nullptr) { *data = Allocator::Allocate<IndexBufferData>(); }

        glCreateBuffers(1, &(*data)->id);

        glNamedBufferStorage((*data)->id, sizeof(uint32_t) * length, indexData, GL_DYNAMIC_STORAGE_BIT);

        (*data)->stride = sizeof(uint32_t);
        (*data)->count = length;
    }

    EXPORT_RENDERER void IndexBufferDestroy(IndexBufferData** data)
    {
        if (*data == nullptr) { return; }

        if ((*data)->id != 0) { glDeleteBuffers(1, &(*data)->id); }
        Allocator::Deallocate(*data);
        *data = nullptr;
    }

    EXPORT_RENDERER void IndexBufferBind(IndexBufferData* data) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->id); }

    EXPORT_RENDERER uint32_t IndexBufferGetID(IndexBufferData* data) { return data->id; }

    EXPORT_RENDERER uint32_t IndexBufferGetSize(IndexBufferData* data) { return data->count * data->stride; }

    EXPORT_RENDERER uint32_t IndexBufferGetLength(IndexBufferData* data) { return data->count; }

}// namespace Engine

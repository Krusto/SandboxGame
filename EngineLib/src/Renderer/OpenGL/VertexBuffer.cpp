#include "StructDefinitions.hpp"
#include <Core/Allocator.hpp>
#include <glad/glad.h>
#include <cassert>
#include <Renderer/Predefines.hpp>
#include <Renderer/Shared/VertexLayout.hpp>

#ifdef __cplusplus
namespace Engine
{
#endif
    inline static void SpecifyVertexArrayAttribute(uint32_t vertexArrayID, uint32_t index, uint32_t offset,
                                                   ShaderUniformType type)
    {
        switch (type)
        {
            case ShaderUniformType::Float:
                glVertexArrayAttribIFormat(vertexArrayID, index, 1, GL_FLOAT, offset);
                break;
            case ShaderUniformType::Int:
                glVertexArrayAttribIFormat(vertexArrayID, index, 1, GL_INT, offset);
                break;
            case ShaderUniformType::UInt:
                glVertexArrayAttribIFormat(vertexArrayID, index, 1, GL_UNSIGNED_INT, offset);
                break;
            case ShaderUniformType::Bool:
                glVertexArrayAttribIFormat(vertexArrayID, index, 1, GL_BOOL, offset);
                break;
            case ShaderUniformType::Vec2:
                glVertexArrayAttribFormat(vertexArrayID, index, 2, GL_FLOAT, GL_FALSE, offset);
                break;
            case ShaderUniformType::Vec3:
                glVertexArrayAttribFormat(vertexArrayID, index, 3, GL_FLOAT, GL_FALSE, offset);
                break;
            case ShaderUniformType::Vec4:
                glVertexArrayAttribFormat(vertexArrayID, index, 4, GL_FLOAT, GL_FALSE, offset);
                break;
            case ShaderUniformType::Mat3:
                glVertexArrayAttribFormat(vertexArrayID, index, 3, GL_FLOAT, GL_FALSE, offset);
                break;
            case ShaderUniformType::Mat4:
                glVertexArrayAttribFormat(vertexArrayID, index, 4, GL_FLOAT, GL_FALSE, offset);
                break;
            case ShaderUniformType::IVec2:
                glVertexArrayAttribIFormat(vertexArrayID, index, 2, GL_INT, offset);
                break;
            case ShaderUniformType::IVec3:
                glVertexArrayAttribIFormat(vertexArrayID, index, 3, GL_INT, offset);
                break;
            case ShaderUniformType::IVec4:
                glVertexArrayAttribIFormat(vertexArrayID, index, 4, GL_INT, offset);
                break;
            case ShaderUniformType::None:
                break;
        }
    }

    EXPORT_RENDERER VertexBufferData* VertexBufferInit(VertexArrayData* vertexArray, VertexLayout* vertexLayout,
                                                       float* vertices, unsigned int length)
    {
        if (length == 0) return nullptr;
        VertexBufferData* data = Allocator::Allocate<VertexBufferData>();

        glCreateBuffers(1, &data->id);
        glNamedBufferStorage(data->id, (GLsizei) (length * vertexLayout->stride), vertices, GL_DYNAMIC_STORAGE_BIT);
        data->size = vertexLayout->stride;
        for (size_t i = 0; i < vertexLayout->attributes.count && vertexLayout->attributes.attributes[i].name != nullptr;
             ++i)
        {
            auto& attr = vertexLayout->attributes.attributes[i];
            glEnableVertexArrayAttrib(vertexArray->id, i);
            glVertexArrayAttribBinding(vertexArray->id, i, 0);
            SpecifyVertexArrayAttribute(vertexArray->id, i, attr.offset, (ShaderUniformType) attr.type);
        }

        return data;
    }

    EXPORT_RENDERER void VertexBufferBind(VertexBufferData* data)
    {
        assert(data->id != 0);
        glBindBuffer(GL_ARRAY_BUFFER, data->id);
    }

    EXPORT_RENDERER size_t VertexBufferGetSize(VertexBufferData* data) { return data->size; }

    EXPORT_RENDERER void VertexBufferDestroy(VertexBufferData** data)
    {
        if (*data == nullptr) { return; }
        if ((*data)->id != 0)
        {
            glDeleteBuffers(1, &(*data)->id);
            (*data)->id = 0;
        }
        Allocator::Deallocate(*data);
        *data = nullptr;
    }

    EXPORT_RENDERER uint32_t VertexBufferGetID(VertexBufferData* data) { return data->id; }
#ifdef __cplusplus
}// namespace Engine
#endif

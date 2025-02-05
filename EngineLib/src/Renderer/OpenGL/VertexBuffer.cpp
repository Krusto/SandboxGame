#include "StructDefinitions.hpp"
#include <Core/Allocator.hpp>
#include <Renderer/Shared/Vertex.hpp>

#include <glad/glad.h>
#include <utility>
#include <cassert>
#include <Renderer/Predefines.hpp>

namespace glm
{
    template <typename T>
    int valtypeToGLtype()
    {
        if (std::is_same<T, float>::value) { return GL_FLOAT; }
        else if (std::is_same<T, uint32_t>::value) { return GL_UNSIGNED_INT; }
        else if (std::is_same<T, int32_t>::value) { return GL_INT; }
        else { return GL_BOOL; }
    }
}// namespace glm

namespace Engine
{
    // void VertexBuffer::Init(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length)
    EXPORT_RENDERER void VertexBufferInit(void** data, void* vertexArray, void* vertexLayout, float* vertices,
                                          unsigned int length)
    {
        if (length == 0) return;
        if (*data == nullptr) { *data = Allocator::Allocate<VertexBufferData>(); }

        VertexBufferData* m_Data = static_cast<VertexBufferData*>(*data);

        VertexLayout& layout = *static_cast<VertexLayout*>(vertexLayout);

        glCreateBuffers(1, &m_Data->id);
        glNamedBufferStorage(m_Data->id, static_cast<GLsizei>(length) * static_cast<GLsizei>(layout.stride), data,
                             GL_DYNAMIC_STORAGE_BIT);

        uint32_t index = 0;
        uint32_t offset = 0;
        uint32_t vaID = asTPtr(vertexArray, VertexArrayData)->id;
        for (auto& attr: layout.attributes)
        {
            glEnableVertexArrayAttrib(vaID, index);
            glVertexArrayAttribBinding(vaID, index, 0);
            switch (attr.type)
            {
                case ShaderUniformType::Float:
                    glVertexArrayAttribIFormat(vaID, index, 1, GL_FLOAT, attr.offset);
                    offset += sizeof(float);
                    break;
                case ShaderUniformType::Int:
                    glVertexArrayAttribIFormat(vaID, index, 1, GL_INT, attr.offset);
                    offset += sizeof(int32_t);
                    break;
                case ShaderUniformType::UInt:
                    glVertexArrayAttribIFormat(vaID, index, 1, GL_UNSIGNED_INT, attr.offset);
                    offset += sizeof(uint32_t);
                    break;
                case ShaderUniformType::Bool:
                    glVertexArrayAttribIFormat(vaID, index, 1, GL_BOOL, attr.offset);
                    offset += sizeof(bool);
                    break;
                case ShaderUniformType::Vec2:
                    glVertexArrayAttribFormat(vaID, index, 2, GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::vec2::value_type) * glm::vec2::length();
                    break;
                case ShaderUniformType::Vec3:
                    glVertexArrayAttribFormat(vaID, index, 3, GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::vec3::value_type) * glm::vec3::length();
                    break;
                case ShaderUniformType::Vec4:
                    glVertexArrayAttribFormat(vaID, index, 4, GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::vec4::value_type) * glm::vec4::length();
                    break;
                case ShaderUniformType::Mat3:
                    glVertexArrayAttribFormat(vaID, index, glm::mat3::length(), GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::mat3::value_type);
                    break;
                case ShaderUniformType::Mat4:
                    glVertexArrayAttribFormat(vaID, index, glm::mat4::length(), GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::mat4::value_type);
                    break;
                case ShaderUniformType::IVec2:
                    glVertexArrayAttribIFormat(vaID, index, 2, GL_INT, attr.offset);
                    offset += sizeof(glm::ivec2::value_type);
                    break;
                case ShaderUniformType::IVec3:
                    glVertexArrayAttribIFormat(vaID, index, 3, GL_INT, attr.offset);
                    offset += sizeof(glm::ivec3::value_type);
                    break;
                case ShaderUniformType::IVec4:
                    glVertexArrayAttribIFormat(vaID, index, 4, GL_INT, attr.offset);
                    offset += sizeof(glm::ivec4::value_type);
                    break;
                case ShaderUniformType::None:
                    break;
            }
            index++;
        }
    }

    EXPORT_RENDERER void VertexBufferBind(void* data)
    {
        VertexBufferData* m_Data = static_cast<VertexBufferData*>(data);
        assert(m_Data->id != 0);
        glBindBuffer(GL_ARRAY_BUFFER, m_Data->id);
    }

    EXPORT_RENDERER size_t VertexBufferGetSize(void* data) { return asTPtr(data, VertexBufferData)->size; }

    EXPORT_RENDERER void VertexBufferDestroy(void** data)
    {
        if (*data == nullptr) { return; }
        if (asTPtr(data, VertexBufferData)->id != 0)
        {
            glDeleteBuffers(1, &asTPtr(data, VertexBufferData)->id);
            asTPtr(data, VertexBufferData)->id = 0;
        }
        Allocator::Deallocate(*data);
        *data = nullptr;
    }

    EXPORT_RENDERER uint32_t VertexBufferGetID(void* data) { return asTPtr(data, VertexBufferData)->id; }

}// namespace Engine

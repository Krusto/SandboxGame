#include <Renderer/Shared/APISpecific/VertexArray.hpp>
#include <Renderer/Shared/APISpecific/VertexBuffer.hpp>

#include <glad/glad.h>
#include <utility>
#include <cassert>

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
    struct VertexBufferData {
        uint32_t m_ID;
    };

    void VertexBuffer::Init(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length)
    {
        m_Data = Allocator::Allocate<VertexBufferData>();

        glCreateBuffers(1, &m_Data->m_ID);
        glNamedBufferStorage(m_Data->m_ID, static_cast<GLsizei>(length) * static_cast<GLsizei>(layout.stride), data,
                             GL_DYNAMIC_STORAGE_BIT);

        uint32_t index = 0;
        uint32_t offset = 0;
        for (auto& attr: layout.attributes)
        {
            glEnableVertexArrayAttrib(va->id(), index);
            glVertexArrayAttribBinding(va->id(), index, 0);
            switch (attr.type)
            {
                case ShaderUniformType::Float:
                    glVertexArrayAttribIFormat(va->id(), index, 1, GL_FLOAT, attr.offset);
                    offset += sizeof(float);
                    break;
                case ShaderUniformType::Int:
                    glVertexArrayAttribIFormat(va->id(), index, 1, GL_INT, attr.offset);
                    offset += sizeof(int32_t);
                    break;
                case ShaderUniformType::UInt:
                    glVertexArrayAttribIFormat(va->id(), index, 1, GL_UNSIGNED_INT, attr.offset);
                    offset += sizeof(uint32_t);
                    break;
                case ShaderUniformType::Bool:
                    glVertexArrayAttribIFormat(va->id(), index, 1, GL_BOOL, attr.offset);
                    offset += sizeof(bool);
                    break;
                case ShaderUniformType::Vec2:
                    glVertexArrayAttribFormat(va->id(), index, 2, GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::vec2::value_type) * glm::vec2::length();
                    break;
                case ShaderUniformType::Vec3:
                    glVertexArrayAttribFormat(va->id(), index, 3, GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::vec3::value_type) * glm::vec3::length();
                    break;
                case ShaderUniformType::Vec4:
                    glVertexArrayAttribFormat(va->id(), index, 4, GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::vec4::value_type) * glm::vec4::length();
                    break;
                case ShaderUniformType::Mat3:
                    glVertexArrayAttribFormat(va->id(), index, glm::mat3::length(), GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::mat3::value_type);
                    break;
                case ShaderUniformType::Mat4:
                    glVertexArrayAttribFormat(va->id(), index, glm::mat4::length(), GL_FLOAT, GL_FALSE, attr.offset);
                    offset += sizeof(glm::mat4::value_type);
                    break;
                case ShaderUniformType::IVec2:
                    glVertexArrayAttribIFormat(va->id(), index, 2, GL_INT, attr.offset);
                    offset += sizeof(glm::ivec2::value_type);
                    break;
                case ShaderUniformType::IVec3:
                    glVertexArrayAttribIFormat(va->id(), index, 3, GL_INT, attr.offset);
                    offset += sizeof(glm::ivec3::value_type);
                    break;
                case ShaderUniformType::IVec4:
                    glVertexArrayAttribIFormat(va->id(), index, 4, GL_INT, attr.offset);
                    offset += sizeof(glm::ivec4::value_type);
                    break;
                case ShaderUniformType::None:
                    break;
            }
            index++;
        }
    }

    void VertexBuffer::Bind() const
    {
        assert(m_Data->m_ID != 0);
        glBindBuffer(GL_ARRAY_BUFFER, m_Data->m_ID);
    }

    size_t VertexBuffer::GetSize() const { return sizeof(VertexBuffer); }

    void VertexBuffer::Destroy()
    {
        if (m_Data == nullptr) { return; }
        if (m_Data->m_ID)
        {
            glDeleteBuffers(1, &m_Data->m_ID);
            m_Data->m_ID = 0;
        }
        Allocator::Deallocate(m_Data);
        m_Data = nullptr;
    }

    uint32_t VertexBuffer::GetID() const { return m_Data->m_ID; }

}// namespace Engine

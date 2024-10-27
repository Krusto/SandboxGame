#include <glad/glad.h>

#include "OpenGLVertexBuffer.hpp"

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
    void OpenGLVertexBuffer::Init(const VertexLayout& layout, float* data, uint32_t length)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(length) * static_cast<GLsizei>(layout.stride), data,
                     GL_STATIC_DRAW);

        uint32_t index = 0;
        uint32_t offset = 0;
        for (auto attr: layout.attributes)
        {
            switch (attr.type)
            {
                case ShaderUniformType::Float:
                    glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(layout.stride),
                                          reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(float);
                    break;
                case ShaderUniformType::Int:
                    glVertexAttribPointer(index, 1, GL_INT, GL_FALSE, static_cast<GLsizei>(layout.stride),
                                          reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(int32_t);
                    break;
                case ShaderUniformType::UInt:
                    glVertexAttribPointer(index, 1, GL_UNSIGNED_INT, GL_FALSE, static_cast<GLsizei>(layout.stride),
                                          reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(uint32_t);
                    break;
                case ShaderUniformType::Bool:
                    glVertexAttribPointer(index, 1, GL_BOOL, GL_FALSE, static_cast<GLsizei>(layout.stride),
                                          reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(bool);
                    break;
                case ShaderUniformType::Vec2:
                    glVertexAttribPointer(index, glm::vec2::length(), glm::valtypeToGLtype<glm::vec2::value_type>(),
                                          GL_FALSE, static_cast<GLsizei>(layout.stride),
                                          reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(glm::vec2::value_type) * glm::vec2::length();
                    break;
                case ShaderUniformType::Vec3:
                    glVertexAttribPointer(index, glm::vec3::length(), glm::valtypeToGLtype<glm::vec3::value_type>(),
                                          GL_FALSE, static_cast<GLsizei>(layout.stride),
                                          reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(glm::vec3::value_type) * glm::vec3::length();
                    break;
                case ShaderUniformType::Vec4:
                    glVertexAttribPointer(index, glm::vec4::length(), glm::valtypeToGLtype<glm::vec4::value_type>(),
                                          GL_FALSE, static_cast<GLsizei>(layout.stride),
                                          reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(glm::vec4::value_type) * glm::vec4::length();
                    break;
                case ShaderUniformType::Mat3:
                    glVertexAttribPointer(index, glm::mat3::length(), glm::valtypeToGLtype<glm::mat3::value_type>(),
                                          GL_FALSE, static_cast<GLsizei>(layout.stride),
                                          reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(glm::mat3::value_type);
                    break;
                case ShaderUniformType::Mat4:
                    glVertexAttribPointer(index, 1, glm::valtypeToGLtype<glm::mat4::value_type>(), GL_FALSE,
                                          static_cast<GLsizei>(layout.stride), reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(glm::mat4::value_type);
                    break;
                case ShaderUniformType::IVec2:
                    glVertexAttribPointer(index, 1, glm::valtypeToGLtype<glm::ivec2::value_type>(), GL_FALSE,
                                          static_cast<GLsizei>(layout.stride), reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(glm::ivec2::value_type);
                    break;
                case ShaderUniformType::IVec3:
                    glVertexAttribPointer(index, 1, glm::valtypeToGLtype<glm::vec3::value_type>(), GL_FALSE,
                                          static_cast<GLsizei>(layout.stride), reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(glm::ivec3::value_type);
                    break;
                case ShaderUniformType::IVec4:
                    glVertexAttribPointer(index, 1, glm::valtypeToGLtype<glm::vec4::value_type>(), GL_FALSE,
                                          static_cast<GLsizei>(layout.stride), reinterpret_cast<void*>(attr.offset));
                    offset += sizeof(glm::ivec4::value_type);
                    break;
                case ShaderUniformType::None:
                    break;
            }
            glEnableVertexAttribArray(index);
            index++;
        }
    }

    void OpenGLVertexBuffer::Bind() const
    {
        assert(m_ID != 0);
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }

    size_t OpenGLVertexBuffer::GetSize() const { return sizeof(OpenGLVertexBuffer); }

    void OpenGLVertexBuffer::Destroy()
    {
        if (m_ID)
        {
            glDeleteBuffers(1, &m_ID);
            m_ID = 0;
        }
    }

}// namespace Engine

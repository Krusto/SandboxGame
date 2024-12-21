#include <Core/Allocator.hpp>
#include <Renderer/Shared/Image.hpp>
#include <glad/glad.h>

namespace Engine
{
    struct ImageData {
        uint32_t id;
    };

    GLint ImageTypeToGL(ImageType type)
    {
        switch (type)
        {
            case ImageType::Depth:
                return GL_DEPTH_COMPONENT;
            case ImageType::RGBA:
                return GL_RGBA;
            case ImageType::RGB:
                return GL_RGB;
            case ImageType::RG:
                return GL_RG;
            default:
                return GL_NONE;
        }
    }

    void Image::Init(uint8_t* data, size_t width, size_t height, ImageType type)
    {
        if (m_Data == nullptr) { m_Data = Engine::Allocator::Allocate<ImageData>(); }
        glGenTextures(1, &m_Data->id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_Data->id);

        glTexImage2D(GL_TEXTURE_2D, 0, ImageTypeToGL(type), (GLsizei) width, (GLsizei) height, 0, ImageTypeToGL(type),
                     GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void Image::Bind(size_t location) const
    {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, m_Data->id);
    }

    void Image::Destroy()
    {
        if (m_Data == nullptr) { return; }
        glDeleteTextures(1, &m_Data->id);
        Allocator::Deallocate(m_Data);
        m_Data = nullptr;
    }

    uint32_t Image::GetID() const { return m_Data->id; }
}// namespace Engine
#include "OpenGLImage.hpp"
#include <glad/glad.h>

namespace Engine
{

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

    void OpenGLImage::Init(uint8_t* data, size_t width, size_t height, ImageType type)
    {
        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ID);

        glTexImage2D(GL_TEXTURE_2D, 0, ImageTypeToGL(type), (GLsizei) width, (GLsizei) height, 0, ImageTypeToGL(type),
                     GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    void OpenGLImage::Bind(size_t location) const
    {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, m_ID);
    }

    void OpenGLImage::Destroy() { glDeleteTextures(1, &m_ID); }

    uint32_t OpenGLImage::GetID() const { return m_ID; }
}// namespace Engine
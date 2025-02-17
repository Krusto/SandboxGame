#include <Core/Allocator.hpp>
#include <Renderer/Shared/ImageType.hpp>
#include <Renderer/OpenGL/ExportedFunctions.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Renderer/Predefines.hpp>

#include <glad/glad.h>

namespace Engine
{

    inline static GLint GLImageFormat(uint8_t format)
    {
        switch (format)
        {
            case ImageColorFormat::Depth:
                return GL_DEPTH_STENCIL;
            case ImageColorFormat::RGBA:
                return GL_RGBA;
            case ImageColorFormat::RGB:
                return GL_RGB;
            case ImageColorFormat::RG:
                return GL_RG;
            case ImageColorFormat::R:
                return GL_RED;
            default:
                return GL_NONE;
        }
    }

    inline static GLint GLComponentType(GLint colorFormat)
    {
        switch (colorFormat)
        {
            case ImageColorFormat::RGBA:
            case ImageColorFormat::RGB:
            case ImageColorFormat::RG:
            case ImageColorFormat::R:
                return GL_FLOAT;
            case ImageColorFormat::Depth:
                return GL_UNSIGNED_INT;
            default:
                return GL_NONE;
        }
    }

    inline static GLint GLStorageFormat(uint8_t colorFormat)
    {
        switch (colorFormat)
        {
            case ImageColorFormat::Depth:
                return GL_DEPTH_COMPONENT24;
            case ImageColorFormat::RGBA:
                return GL_RGBA8;
            case ImageColorFormat::RGB:
                return GL_RGB8;
            case ImageColorFormat::RG:
                return GL_RG8;
            case ImageColorFormat::R:
                return GL_R8;
            default:
                return GL_NONE;
        }
    }

    EXPORT_RENDERER void ImageInit(void** data, uint8_t* imageData, size_t width, size_t height, uint8_t format,
                                   uint8_t type)
    {
        if (*data == nullptr) { *data = (void*) Engine::Allocator::Allocate<ImageData>(); }
        ImageData* m_ImageData = (ImageData*) *data;
        glCreateTextures(GL_TEXTURE_2D, 1, &m_ImageData->id);
        glTextureStorage2D(m_ImageData->id, 1, GLStorageFormat(format), (GLsizei) width, (GLsizei) height);
        if (imageData)
        {
            glTextureSubImage2D(m_ImageData->id, 0, 0, 0, (GLsizei) width, (GLsizei) height, GLImageFormat(format),
                                GLComponentType(format), imageData);
        }
        glTextureParameteri(m_ImageData->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_ImageData->id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_ImageData->id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_ImageData->id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    EXPORT_RENDERER void ImageBind(ImageData* data, size_t location) { glBindTextureUnit(location, data->id); }

    EXPORT_RENDERER void ImageDestroy(void** data)
    {
        if (*data == nullptr) { return; }
        glDeleteTextures(1, &asTPtr(*data, ImageData)->id);
        Allocator::Deallocate(*data);
        *data = nullptr;
    }

    EXPORT_RENDERER uint32_t ImageGetID(void* data) { return asTPtr(data, ImageData)->id; }
}// namespace Engine
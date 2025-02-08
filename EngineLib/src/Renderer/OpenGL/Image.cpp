#include <Core/Allocator.hpp>
#include <Renderer/Shared/ImageType.hpp>
#include <Renderer/OpenGL/ExportedFunctions.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Renderer/Predefines.hpp>

#include <glad/glad.h>

namespace Engine
{

    EXPORT_RENDERER GLint ImageTypeToGL(uint8_t type)
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

    EXPORT_RENDERER void ImageInit(void** data, uint8_t* imageData, size_t width, size_t height, uint8_t type)
    {
        if (*data == nullptr) { *data = Engine::Allocator::Allocate<ImageData>(); }
        glGenTextures(1, &asTPtr(*data, ImageData)->id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, asTPtr(*data, ImageData)->id);

        glTexImage2D(GL_TEXTURE_2D, 0, ImageTypeToGL(type), (GLsizei) width, (GLsizei) height, 0, ImageTypeToGL(type),
                     GL_FLOAT, imageData);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    EXPORT_RENDERER void ImageBind(void* data, size_t location)
    {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, asTPtr(data, ImageData)->id);
    }

    EXPORT_RENDERER void ImageDestroy(void** data)
    {
        if (*data == nullptr) { return; }
        glDeleteTextures(1, &asTPtr(*data, ImageData)->id);
        Allocator::Deallocate(*data);
        *data = nullptr;
    }

    EXPORT_RENDERER uint32_t ImageGetID(void* data) { return asTPtr(data, ImageData)->id; }
}// namespace Engine
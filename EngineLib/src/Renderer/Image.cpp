#include "Image.hpp"
#include <Core/Allocator.hpp>
#include <Renderer/OpenGL/OpenGLImage.hpp>

namespace Engine
{
    Image* Image::Create(uint8_t* data, size_t width, size_t height, ImageType type)
    {
        auto image = Allocator::Allocate<OpenGLImage>();
        image->Init(data, width, height, type);
        return image;
    }
}// namespace Engine
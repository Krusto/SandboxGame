#include "Image.hpp"

namespace Engine
{
    Image Image::Create(uint8_t* data, size_t width, size_t height, ImageType type)
    {
        Image image;
        image.Init(data, width, height, type);
        return image;
    }
}// namespace Engine
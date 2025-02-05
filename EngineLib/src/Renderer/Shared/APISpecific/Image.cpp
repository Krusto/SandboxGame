#include "Image.hpp"
#include <Renderer/Shared/function_pointers.h>

namespace Engine
{
    Image Image::Create(uint8_t* data, size_t width, size_t height, ImageType type)
    {
        Image image;
        image.Init(data, width, height, type);
        return image;
    }

    void Image::Init(uint8_t* data, size_t width, size_t height, ImageType type)
    {
        ImageInit((void**) &m_Data, data, width, height, (uint8_t) type);
    }

    void Image::Bind(size_t location) const { ImageBind((void*) m_Data, location); }

    void Image::Destroy() { ImageDestroy((void**) &m_Data); }

    uint32_t Image::GetID() const { return ImageGetID((void*) m_Data); }
}// namespace Engine
#include "Image.hpp"
#include <Renderer/Renderer.hpp>

namespace Engine
{
    Image::Image(uint8_t* data, size_t width, size_t height, ImageColorFormat format, ImageType type)
    {
        Init(data, width, height, format, type);
    }

    Image Image::Create(uint8_t* data, size_t width, size_t height, ImageColorFormat format, ImageType type)
    {
        return Image(data, width, height, format, type);
    }

    void Image::Init(uint8_t* data, size_t width, size_t height, ImageColorFormat format, ImageType type)
    {
        Renderer::GetInstance()->ImageInit((void**) &m_Data, data, width, height, (uint8_t) format, (uint8_t) type);
    }

    void Image::Bind(size_t location) const { Renderer::GetInstance()->ImageBind((void*) m_Data, location); }

    void Image::Destroy() { Renderer::GetInstance()->ImageDestroy((void**) &m_Data); }

    uint32_t Image::GetID() const { return Renderer::GetInstance()->ImageGetID((void*) m_Data); }
}// namespace Engine
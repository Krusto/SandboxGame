#pragma once
#include <cstdint>
#include <Renderer/Predefines.hpp>

namespace Engine
{
    enum class ImageType
    {
        RGBA,
        RGB,
        RG,
        Depth
    };

    class EXPORT_RENDERER Image
    {
    public:
        static Image* Create(uint8_t* data, size_t width, size_t height, ImageType type);

    public:
        virtual ~Image() {}

        virtual void Init(uint8_t* data, size_t width, size_t height, ImageType type) = 0;
        virtual void Bind(size_t location) const = 0;
        virtual void Destroy() = 0;
        virtual uint32_t GetID() const = 0;
    };
}// namespace Engine
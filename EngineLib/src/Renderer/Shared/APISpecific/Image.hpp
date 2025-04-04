#pragma once
#include <Core/Types.hpp>

namespace Engine
{
    enum class ImageColorFormat : uint8_t
    {
        RGBA = 0,
        RGB,
        RG,
        R,
        Depth
    };
    enum class ImageType : uint8_t
    {
        STATIC = 0,
        DYNAMIC,
    };

    struct ImageData;

    class Image
    {
    public:
        Image() = default;
        Image(uint8_t* data, std::size_t width, std::size_t height, ImageColorFormat format, ImageType type);
        ~Image() = default;

        static Image Create(uint8_t* data, std::size_t width, std::size_t height, ImageColorFormat format,
                            ImageType type);

    public:
        void Init(uint8_t* data, std::size_t width, std::size_t height, ImageColorFormat format, ImageType type);
        void Bind(std::size_t location) const;
        void Destroy();
        uint32_t GetID() const;

    private:
        ImageData* m_Data{};
    };
}// namespace Engine
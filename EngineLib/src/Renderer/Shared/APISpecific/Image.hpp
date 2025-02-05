#pragma once
#include <cstdint>

namespace Engine
{
    enum class ImageType : uint8_t
    {
        RGBA,
        RGB,
        RG,
        Depth
    };
    struct ImageData;

    class Image
    {
    public:
        Image() = default;
        ~Image() = default;
        static Image Create(uint8_t* data, size_t width, size_t height, ImageType type);

    public:
        void Init(uint8_t* data, size_t width, size_t height, ImageType type);
        void Bind(size_t location) const;
        void Destroy();
        uint32_t GetID() const;

    private:
        ImageData* m_Data{};
    };
}// namespace Engine
#pragma once
#include <cstdint>
#include <Renderer/Image.hpp>

namespace Engine
{
    class OpenGLImage: public Image
    {
    public:
        OpenGLImage() = default;
        ~OpenGLImage() = default;

    public:
        void Init(uint8_t* data, size_t width, size_t height, ImageType type) override;
        void Bind(size_t location) const override;
        void Destroy() override;
        uint32_t GetID() const override;

    private:
        uint32_t m_ID{};
    };
}// namespace Engine
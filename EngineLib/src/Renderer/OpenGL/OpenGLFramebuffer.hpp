#pragma once
#include <cstdint>
#include <Renderer/Framebuffer.hpp>

namespace Engine
{

    class OpenGLFramebuffer: public Framebuffer
    {
    public:
        OpenGLFramebuffer() = default;
        ~OpenGLFramebuffer() = default;

    public:
        void Init(uint32_t width, uint32_t height);

    public:
        virtual uint32_t GetID() override;

        virtual uint32_t& GetColorAttachmentID() override;

        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual void Bind() override;

        virtual void Unbind() override;

        virtual void Destroy() override;

        virtual uint32_t width() override;

        virtual uint32_t height() override;

        virtual ViewportSize GetViewportSize() override;

    private:
        uint32_t m_width{};
        uint32_t m_height{};
        uint32_t m_fbo{};
        uint32_t m_color_texture{};
        uint32_t m_depth_texture{};
    };
}// namespace Engine
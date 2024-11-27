#pragma once
#include <Renderer/Viewport.hpp>
#include <Renderer/RendererCommand.hpp>

namespace Engine
{
    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;

    public:
        static Framebuffer* Create(uint32_t width, uint32_t height, bool isDepthMap = false);

    public:
        RendererCommand BindCommand() const;

    public:
        virtual uint32_t GetID() = 0;

        virtual uint32_t GetColorAttachmentID() = 0;

        virtual uint32_t GetDepthAttachmentID() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual void Destroy() = 0;

        virtual void Bind() const = 0;

        virtual void Unbind() = 0;

        virtual uint32_t width() = 0;

        virtual uint32_t height() = 0;

        virtual ViewportSize GetViewportSize() = 0;
    };
}// namespace Engine
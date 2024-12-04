#pragma once
#include <Renderer/Viewport.hpp>
#include <Renderer/RendererCommand.hpp>

#include <glm/glm.hpp>
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
        RendererCommand BindColorTextureCommand(uint32_t slot) const;
        RendererCommand BindDepthTextureCommand(uint32_t slot) const;
        RendererCommand ClearColorCommand(glm::vec4 color) const;
        RendererCommand ClearDepthCommand() const;

    public:
        virtual uint32_t GetID() = 0;

        virtual uint32_t GetColorAttachmentID() = 0;

        virtual uint32_t GetDepthAttachmentID() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual void Destroy() = 0;

        virtual void Bind() const = 0;

        virtual void BindColorTexture(uint32_t slot) const = 0;

        virtual void BindDepthTexture(uint32_t slot) const = 0;

        virtual void ClearColor(glm::vec4 color) const = 0;

        virtual void ClearDepth() const = 0;

        virtual void Unbind() = 0;

        virtual uint32_t width() = 0;

        virtual uint32_t height() = 0;

        virtual ViewportSize GetViewportSize() = 0;
    };
}// namespace Engine
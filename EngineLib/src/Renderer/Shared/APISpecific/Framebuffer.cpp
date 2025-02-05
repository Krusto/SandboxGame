#include "Framebuffer.hpp"
#include <Renderer/Shared/function_pointers.h>

namespace Engine
{
    Framebuffer Framebuffer::Create(uint32_t width, uint32_t height, bool isDepthMap)
    {
        Framebuffer framebufferPtr;
        framebufferPtr.Init(width, height, isDepthMap);
        return framebufferPtr;
    }

    RendererCommand Framebuffer::BindCommand() const
    {
        return RendererCommand([=]() { Bind(); });
    }

    RendererCommand Framebuffer::BindColorTextureCommand(uint32_t slot) const
    {
        return RendererCommand([=]() { BindColorTexture(slot); });
    }

    RendererCommand Framebuffer::BindDepthTextureCommand(uint32_t slot) const
    {
        return RendererCommand([=]() { BindDepthTexture(slot); });
    }

    RendererCommand Framebuffer::ClearColorCommand(glm::vec4 color) const
    {
        return RendererCommand([=]() { ClearColor(color); });
    }

    RendererCommand Framebuffer::ClearDepthCommand() const
    {
        return RendererCommand([=]() { ClearDepth(); });
    }

    void Framebuffer::Init(uint32_t width, uint32_t height, bool isDepthMap)
    {

        FramebufferInit((void**) &m_Data, width, height, isDepthMap);
    }

    void Framebuffer::Destroy() { FramebufferDestroy((void**) &m_Data); }

    uint32_t Framebuffer::GetID() { return FramebufferGetID(m_Data); }

    uint32_t Framebuffer::GetColorAttachmentID() { return FramebufferGetColorTextureID(m_Data); }

    uint32_t Framebuffer::GetDepthAttachmentID() { return FramebufferGetDepthTextureID(m_Data); }

    void Framebuffer::Resize(uint32_t width, uint32_t height)
    {
        FramebufferDestroy((void**) &m_Data);
        FramebufferInit((void**) &m_Data, width, height, false);
    }

    void Framebuffer::Bind() const { FramebufferBind(m_Data); }

    void Framebuffer::BindColorTexture(uint32_t slot) const { FramebufferBindColorTexture(m_Data, slot); }

    void Framebuffer::BindDepthTexture(uint32_t slot) const { FramebufferBindDepthTexture(m_Data, slot); }

    void Framebuffer::ClearColor(glm::vec4 color) const { FramebufferClearColor(m_Data, (void*) &color); }

    void Framebuffer::ClearDepth() const { FramebufferClearDepth(m_Data); }

    void Framebuffer::Unbind() { FramebufferUnbind(m_Data); }

    uint32_t Framebuffer::width() { return FramebufferGetWidth(m_Data); }

    uint32_t Framebuffer::height() { return FramebufferGetHeight(m_Data); }

    ViewportSize Framebuffer::GetViewportSize() { return {width(), height()}; }

}// namespace Engine
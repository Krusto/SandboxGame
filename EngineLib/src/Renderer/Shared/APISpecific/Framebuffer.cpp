#include "Framebuffer.hpp"
#include "Renderer/Renderer.hpp"

namespace Engine
{
    Framebuffer Framebuffer::Create(uint32_t width, uint32_t height, bool isDepthMap)
    {
        return {width, height, isDepthMap};
    }

    Framebuffer::Framebuffer(uint32_t width, uint32_t height, bool isDepthMap)
    {
        Renderer::GetInstance()->FramebufferInit((void**) &m_Data, width, height, isDepthMap);
    }

    RendererCommand Framebuffer::BindCommand() const
    {
        return RendererCommand([fbo = this]() { fbo->Bind(); });
    }

    RendererCommand Framebuffer::BindColorTextureCommand(uint32_t slot) const
    {
        return RendererCommand([fbo = this, slot]() { fbo->BindColorTexture(slot); });
    }

    RendererCommand Framebuffer::BindDepthTextureCommand(uint32_t slot) const
    {
        return RendererCommand([fbo = this, slot]() { fbo->BindDepthTexture(slot); });
    }

    RendererCommand Framebuffer::ClearColorCommand(glm::vec4 color) const
    {
        return RendererCommand([fbo = this, color]() { fbo->ClearColor(color); });
    }

    RendererCommand Framebuffer::ClearDepthCommand() const
    {
        return RendererCommand([fbo = this]() { fbo->ClearDepth(); });
    }

    void Framebuffer::Init(uint32_t width, uint32_t height, bool isDepthMap)
    {

        Renderer::GetInstance()->FramebufferInit((void**) &m_Data, width, height, isDepthMap);
    }

    void Framebuffer::Destroy() { Renderer::GetInstance()->FramebufferDestroy((void**) &m_Data); }

    uint32_t Framebuffer::GetID() { return Renderer::GetInstance()->FramebufferGetID(m_Data); }

    uint32_t Framebuffer::GetColorAttachmentID() const
    {
        return Renderer::GetInstance()->FramebufferGetColorTextureID(m_Data);
    }

    uint32_t Framebuffer::GetDepthAttachmentID()
    {
        return Renderer::GetInstance()->FramebufferGetDepthTextureID(m_Data);
    }

    void Framebuffer::Resize(uint32_t width, uint32_t height)
    {
        Renderer::GetInstance()->FramebufferDestroy((void**) &m_Data);
        Renderer::GetInstance()->FramebufferInit((void**) &m_Data, width, height, false);
    }

    void Framebuffer::Bind() const { Renderer::GetInstance()->FramebufferBind(m_Data); }

    void Framebuffer::BindColorTexture(uint32_t slot) const
    {
        Renderer::GetInstance()->FramebufferBindColorTexture(m_Data, slot);
    }

    void Framebuffer::BindDepthTexture(uint32_t slot) const
    {
        Renderer::GetInstance()->FramebufferBindDepthTexture(m_Data, slot);
    }

    void Framebuffer::ClearColor(glm::vec4 color) const
    {
        Renderer::GetInstance()->FramebufferClearColor(m_Data, (void*) &color);
    }

    void Framebuffer::ClearDepth() const { Renderer::GetInstance()->FramebufferClearDepth(m_Data); }

    void Framebuffer::Unbind() { Renderer::GetInstance()->FramebufferUnbind(m_Data); }

    uint32_t Framebuffer::width() { return Renderer::GetInstance()->FramebufferGetWidth(m_Data); }

    uint32_t Framebuffer::height() { return Renderer::GetInstance()->FramebufferGetHeight(m_Data); }

    ViewportSize Framebuffer::GetViewportSize() { return {width(), height()}; }

}// namespace Engine
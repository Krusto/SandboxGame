#include <Renderer/RendererAPI.hpp>
#include <Renderer/OpenGL/OpenGLFramebuffer.hpp>
#include "Framebuffer.hpp"
#include <Core/Allocator.hpp>

namespace Engine
{
    Framebuffer* Framebuffer::Create(uint32_t width, uint32_t height, bool isDepthMap)
    {
        OpenGLFramebuffer* framebufferPtr = Engine::Allocator::Allocate<OpenGLFramebuffer>();
        if (framebufferPtr) framebufferPtr->Init(width, height, isDepthMap);
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
}// namespace Engine
#include <Renderer/RendererAPI.hpp>
#include <Renderer/OpenGL/OpenGLFramebuffer.hpp>
#include "Framebuffer.hpp"
#include <Core/Allocator.hpp>

namespace Engine
{
    Framebuffer* Framebuffer::Create(uint32_t width, uint32_t height)
    {
        OpenGLFramebuffer* framebufferPtr = Engine::Allocator::Allocate<OpenGLFramebuffer>();
        if (framebufferPtr) framebufferPtr->Init(width, height);
        return framebufferPtr;
    }

    RendererCommand Framebuffer::BindCommand() const
    {
        return RendererCommand([=]() { Bind(); });
    }
}// namespace Engine
#include <Renderer/RendererAPI.hpp>
#include <Renderer/OpenGL/OpenGLFramebuffer.hpp>
#include "Framebuffer.hpp"

namespace Engine
{
    Framebuffer* Framebuffer::Create(uint32_t width, uint32_t height)
    {
        auto framebufferPtr = new OpenGLFramebuffer();
        if (framebufferPtr) framebufferPtr->Init(width, height);
        return framebufferPtr;
    }
}// namespace Engine
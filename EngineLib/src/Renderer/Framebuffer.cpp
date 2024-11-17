#include <Renderer/RendererAPI.hpp>
#include <Renderer/OpenGL/OpenGLFramebuffer.hpp>
#include "Framebuffer.hpp"
#include <Core/Allocator.hpp>

namespace Engine
{
    Framebuffer* Framebuffer::Create(uint32_t width, uint32_t height)
    {

        Allocate(OpenGLFramebuffer, framebufferPtr);
        if (framebufferPtr) framebufferPtr->Init(width, height);
        return framebufferPtr;
    }
}// namespace Engine
#include <Core/Ref.hpp>
#include <Renderer/RendererAPI.hpp>
#include <Renderer/OpenGL/OpenGLFramebuffer.hpp>
#include "Framebuffer.hpp"

namespace Engine
{
    Ref<Framebuffer> Framebuffer::Create(uint32_t width, uint32_t height)
    {
        return Ref<OpenGLFramebuffer>::Create(width, height);
    }
}// namespace Engine
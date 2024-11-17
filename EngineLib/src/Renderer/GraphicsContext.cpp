#include "GraphicsContext.hpp"
#include <Renderer/OpenGL/OpenGLContext.hpp>
#include <Core/Allocator.hpp>
GraphicsContext* GraphicsContext::s_Context;

GraphicsContext* GraphicsContext::Create(GLFWwindow* window)
{
    Allocate(OpenGLContext, ptr, window);

    return ptr;
}

void GraphicsContext::Destroy() { Deallocate(GraphicsContext::s_Context); }
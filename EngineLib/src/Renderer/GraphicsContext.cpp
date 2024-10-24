#include "GraphicsContext.hpp"
#include <Renderer/OpenGL/OpenGLContext.hpp>
GraphicsContext* GraphicsContext::s_Context;

GraphicsContext* GraphicsContext::Create(GLFWwindow* window)
{
    auto context = new OpenGLContext(window);
    context->window = window;
    return context;
}

#include "GraphicsContext.hpp"
#include <Renderer/OpenGL/OpenGLContext.hpp>
#include <Core/Allocator.hpp>
GraphicsContext* GraphicsContext::s_Context;

GraphicsContext* GraphicsContext::Create(GLFWwindow* window)
{
    GraphicsContext* ptr = (GraphicsContext*) Engine::Allocator::Allocate < OpenGLContext>(window);

    return ptr;
}

void GraphicsContext::Destroy() { Engine::Allocator::Deallocate(GraphicsContext::s_Context); }
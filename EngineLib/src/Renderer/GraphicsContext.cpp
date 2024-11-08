#include "GraphicsContext.hpp"
#include <Renderer/OpenGL/OpenGLContext.hpp>
#include <Core/Allocator.hpp>
GraphicsContext* GraphicsContext::s_Context;

GraphicsContext* GraphicsContext::Create(GLFWwindow* window)
{


    return Engine::Allocator::Allocate<OpenGLContext>(window);
}

void GraphicsContext::Destroy() { Engine::Allocator::Deallocate(GraphicsContext::s_Context); }
#include "GraphicsContext.hpp"
#include <Core/Allocator.hpp>
GraphicsContext* GraphicsContext::s_Context;

GraphicsContext* GraphicsContext::Create(GLFWwindow* window)
{
    GraphicsContext* ptr = (GraphicsContext*) Engine::Allocator::Allocate<GraphicsContext>(window);

    return ptr;
}

void GraphicsContext::Destroy() { Engine::Allocator::Deallocate(GraphicsContext::s_Context); }
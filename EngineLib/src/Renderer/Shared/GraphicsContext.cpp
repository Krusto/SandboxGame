#include "GraphicsContext.hpp"
#include <Core/Allocator.hpp>

EXPORT_RENDERER GraphicsContext s_Context;

void GraphicsContext::Create(GLFWwindow* window) { s_Context.p_WindowPtr = window; }

GraphicsContext* GraphicsContext::Get() { return &s_Context; }

void GraphicsContext::Destroy() { s_Context.p_WindowPtr = nullptr; }
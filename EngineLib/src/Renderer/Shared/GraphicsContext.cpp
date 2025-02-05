#include "GraphicsContext.hpp"

#include <Renderer/Renderer.hpp>

GraphicsContext s_Context;

GraphicsContext* GraphicsContext::Get() { return &s_Context; }

void GraphicsContext::Create(GLFWwindow* window)
{
    s_Context.p_WindowPtr = window;

    Engine::Renderer::RendererContextCreate(this, window);
}

void GraphicsContext::Destroy()
{
    s_Context.p_WindowPtr = nullptr;
    Engine::Renderer::RendererContextDestroy(this);
}

void GraphicsContext::Init() { Engine::Renderer::RendererContextInit(this); }

void GraphicsContext::SwapBuffers() { Engine::Renderer::RendererContextSwapBuffers(p_WindowPtr); }

void GraphicsContext::AddDebugMessanger() { Engine::Renderer::RendererContextAddDebugMessanger(p_WindowPtr); }

void GraphicsContext::SetupWindowHints() 
{ 
    //Engine::Renderer::RendererContextSetupWindowHints(nullptr); 
}
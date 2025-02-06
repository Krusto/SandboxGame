#include "GraphicsContext.hpp"

#include <Renderer/Renderer.hpp>

GraphicsContext s_Context;

GraphicsContext* GraphicsContext::Get() { return &s_Context; }

void GraphicsContext::Create(GLFWwindow* window)
{
    s_Context.p_WindowPtr = window;

    Engine::Renderer::GetInstance()->RendererContextCreate(Get(), window);
}

void GraphicsContext::Destroy()
{
    s_Context.p_WindowPtr = nullptr;
    Engine::Renderer::GetInstance()->RendererContextDestroy(this);
}

void GraphicsContext::Init() { Engine::Renderer::GetInstance()->RendererContextInit(this); }

void GraphicsContext::SwapBuffers() { Engine::Renderer::GetInstance()->RendererContextSwapBuffers(p_WindowPtr); }

void GraphicsContext::AddDebugMessanger() { Engine::Renderer::GetInstance()->RendererContextAddDebugMessanger(p_WindowPtr); }

void GraphicsContext::SetupWindowHints() 
{ 
    //Engine::Renderer::GetInstance()->RendererContextSetupWindowHints(nullptr); 
}
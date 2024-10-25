#include "Renderer.hpp"

#include <Renderer/OpenGL/OpenGLRenderer.hpp>
#include <Renderer/OpenGL/OpenGLContext.hpp>

#include <iostream>

namespace Engine
{
    static RendererAPI* s_RendererAPI = nullptr;

    static RendererAPI* CreateRendererAPIInstance()
    {
        return new OpenGLRenderer();
        return nullptr;
    }

    RendererAPI* Renderer::GetAPIInstance() { return s_RendererAPI; }

    void Renderer::Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec)
    {
        s_RendererAPI = CreateRendererAPIInstance();

        if (s_RendererAPI) s_RendererAPI->Init(rendererSpec, applicationSpec);
    }

    void Renderer::InitImGUI(std::weak_ptr<Window> window) {}

    void Renderer::Shutdown()
    {
        if (s_RendererAPI) s_RendererAPI->Shutdown();
    }

    void Renderer::ClearColor(glm::vec4 color)
    {
        if (s_RendererAPI) s_RendererAPI->ClearColor(color);
    }

    GraphicsContext* Renderer::CreateGraphicsContext(GLFWwindow* handle) { return GraphicsContext::Create(handle); }

}// namespace Engine
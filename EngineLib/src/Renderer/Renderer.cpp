#include "Renderer.hpp"

#include <Renderer/OpenGL/OpenGLRenderer.hpp>
#include <Renderer/OpenGL/OpenGLContext.hpp>

namespace Engine
{
    static RendererAPI* s_RendererAPI = nullptr;

    RendererAPI* Renderer::GetAPIInstance() { return s_RendererAPI; }

    void Renderer::Submit(const std::function<void(void)>& func) { s_FunctionContainer.push_back(func); }

    void Renderer::Flush()
    {
        for (auto& function: s_FunctionContainer) { function(); }
        s_FunctionContainer.clear();
    }

    void Renderer::Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec)
    {
        s_RendererAPI = new OpenGLRenderer();

        if (s_RendererAPI) s_RendererAPI->Init(rendererSpec, applicationSpec);
    }

    void Renderer::InitImGUI(std::weak_ptr<Window> window) {}

    void Renderer::Shutdown()
    {
        if (s_RendererAPI) s_RendererAPI->Shutdown();
    }

    void Renderer::BeginFrame(){};

    void Renderer::EndFrame(){};

    void Renderer::ClearColor(glm::vec4 color)
    {
        if (s_RendererAPI) s_RendererAPI->ClearColor(color);
    }

    GraphicsContext* Renderer::CreateGraphicsContext(GLFWwindow* handle) { return GraphicsContext::Create(handle); }
}// namespace Engine
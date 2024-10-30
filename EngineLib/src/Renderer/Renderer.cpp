#include "Renderer.hpp"

#include <Renderer/OpenGL/OpenGLRenderer.hpp>
#include <Renderer/OpenGL/OpenGLContext.hpp>

namespace Engine
{
    static RendererAPI* s_RendererAPI = nullptr;

    RendererAPI* Renderer::GetAPIInstance() { return s_RendererAPI; }

    void Renderer::Submit(RendererCommand command) { s_CommandContainer.push_back(command); }

    void Renderer::SubmitAndFlush(RendererCommand command)
    {
        s_CommandContainer.push_back(command);
        Renderer::Flush();
    }

    void Renderer::Flush()
    {
        for (auto& command: s_CommandContainer) { command.Execute(); }
        s_CommandContainer.clear();
    }

    void Renderer::Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec)
    {
        s_RendererAPI = new OpenGLRenderer();

        if (s_RendererAPI) s_RendererAPI->Init(rendererSpec, applicationSpec);
    }

    void Renderer::Destroy()
    {
        if (s_RendererAPI) delete s_RendererAPI;
    }

    void Renderer::InitImGUI(Ref<Window> window) {}

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
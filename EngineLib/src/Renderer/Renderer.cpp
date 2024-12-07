#include "Renderer.hpp"

#include <Renderer/OpenGL/OpenGLRenderer.hpp>
#include <Renderer/OpenGL/OpenGLContext.hpp>
#include <Core/Allocator.hpp>

namespace Engine
{
    static RendererAPI* s_RendererAPI = nullptr;

    RendererAPI* Renderer::GetAPIInstance() { return s_RendererAPI; }

    void Renderer::SwitchWireframeMode()
    {
        if (s_RendererAPI) s_RendererAPI->SwitchMode(1);
    }

    void Renderer::SwitchFillMode()
    {
        if (s_RendererAPI) s_RendererAPI->SwitchMode(0);
    }

    void Renderer::SubmitRenderIndexed(VertexArray* vertexArray, uint32_t indexCount)
    {
        if (s_RendererAPI)
        {
            Renderer::Submit(RendererCommand([=]() { s_RendererAPI->RenderIndexed(vertexArray, indexCount); }));
        }
    }

    void Renderer::RenderIndexed(const VertexArray* vertexArray, uint32_t indexCount)
    {
        if (s_RendererAPI) { s_RendererAPI->RenderIndexed(vertexArray, indexCount); }
    }

    void Renderer::SetViewport(ViewportSize size)
    {
        if (s_RendererAPI)
        {
            Renderer::Submit(RendererCommand([=]() { s_RendererAPI->SetViewport(size); }));
        }
    }

    void Renderer::BindDefaultFramebuffer()
    {
        if (s_RendererAPI)
        {
            Renderer::Submit(RendererCommand([=]() { s_RendererAPI->BindDefaultFramebuffer(); }));
        }
    }

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
        s_RendererAPI = Engine::Allocator::Allocate<OpenGLRenderer>();

        if (s_RendererAPI) s_RendererAPI->Init(rendererSpec, applicationSpec);
    }

    void Renderer::Destroy()
    {
        if (s_RendererAPI) Engine::Allocator::Deallocate(s_RendererAPI);
    }

    void Renderer::InitImGUI(Window* window) {}

    void Renderer::Shutdown()
    {
        if (s_RendererAPI) s_RendererAPI->Shutdown();
    }

    void Renderer::BeginFrame(){};

    void Renderer::EndFrame(){};

    GraphicsContext* Renderer::CreateGraphicsContext(GLFWwindow* handle) { return GraphicsContext::Create(handle); }
}// namespace Engine
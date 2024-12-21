#include "Renderer.hpp"
#include <Renderer/Shared/APISpecific/RendererAPI.hpp>

#include <Application/ApplicationSpec.hpp>
#include <Renderer/Shared/GraphicsContext.hpp>
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

    void Renderer::ChangeDepthFunction(DepthFunction depthFunction)
    {
        if (s_RendererAPI) s_RendererAPI->ChangeDepthFunction(depthFunction);
    }

    void Renderer::SubmitRenderIndexed(VertexArray vertexArray, uint32_t indexCount)
    {
        if (s_RendererAPI)
        {
            Renderer::Submit(RendererCommand([=]() { s_RendererAPI->RenderIndexed(vertexArray, indexCount); }));
        }
    }

    void Renderer::RenderIndexed(VertexArray vertexArray, uint32_t indexCount)
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
        s_RendererAPI = Allocator::Allocate<RendererAPI>();

        if (s_RendererAPI) s_RendererAPI->Init(rendererSpec, applicationSpec);
    }

    void Renderer::Destroy()
    {
        if (s_RendererAPI) Engine::Allocator::Deallocate(s_RendererAPI);
    }

    void Renderer::InitImGUI(GLFWwindow* window)
    {
        if (s_RendererAPI) s_RendererAPI->InitImGUI(window);
    }

    void Renderer::DestroyImGUI()
    {
        if (s_RendererAPI) s_RendererAPI->DestroyImGUI();
    }

    void Renderer::Shutdown()
    {
        if (s_RendererAPI) s_RendererAPI->Shutdown();
    }

    void Renderer::ImGuiNewFrame()
    {
        if (s_RendererAPI) s_RendererAPI->ImGuiNewFrame();
    }

    void Renderer::ImGuiRender(ImDrawData* drawData)
    {
        if (s_RendererAPI) s_RendererAPI->ImGuiRender(drawData);
    }

    void Renderer::BeginFrame(){};

    void Renderer::EndFrame(){};

}// namespace Engine
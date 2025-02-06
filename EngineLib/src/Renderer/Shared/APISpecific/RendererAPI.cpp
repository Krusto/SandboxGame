#include "RendererAPI.hpp"
#include <Renderer/Renderer.hpp>
#include <Application/ApplicationSpec.hpp>

namespace Engine
{

    void RendererAPI::ClearColor(glm::vec4 color)
    {
        Renderer::GetInstance()->RendererAPISetClearColor(m_Data, color.r, color.g, color.b, color.a);
    }

    void RendererAPI::Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec)
    {
        Renderer::GetInstance()->RendererAPIInit((void**) &m_Data, &rendererSpec, &applicationSpec);
    }

    void RendererAPI::Shutdown() { Renderer::GetInstance()->RendererAPIDestroy((void**) &m_Data); }

    void RendererAPI::BeginFrame() {}

    void RendererAPI::EndFrame() {}

    void RendererAPI::SwitchMode(uint32_t mode) { Renderer::GetInstance()->RendererAPISwitchPolygonMode(m_Data, mode); }

    void RendererAPI::SetViewport(ViewportSize size)
    {
        Renderer::GetInstance()->RendererAPISetViewport(m_Data, size.width, size.height);
    }

    void RendererAPI::BindDefaultFramebuffer() const { Renderer::GetInstance()->RendererAPIBindDefaultFramebuffer(m_Data); }

    void RendererAPI::RenderIndexed(VertexArray vertexArray, uint32_t indexCount) const
    {
        vertexArray.Bind();
        Renderer::GetInstance()->RendererAPIRenderIndexed(m_Data, indexCount);
    };

    void RendererAPI::ChangeDepthFunction(DepthFunction depthFunction)
    {
        Renderer::GetInstance()->RendererAPIChangeDepthFunc(m_Data, depthFunction);
    }

    void RendererAPI::InitImGUI(GLFWwindow* window) { Renderer::GetInstance()->RendererAPIInitIMGUI(Renderer::GetInstance(),m_Data, window); }

    void RendererAPI::DestroyImGUI() { Renderer::GetInstance()->RendererAPIDestroyIMGUI(m_Data); }

    void RendererAPI::ImGuiNewFrame() { Renderer::GetInstance()->RendererAPIIMGUIBegin(m_Data); }

    void RendererAPI::ImGuiRender(ImDrawData* drawData) { Renderer::GetInstance()->RendererAPIIMGUIEnd(m_Data, drawData); }

    RendererAPIType RendererAPI::GetAPI() const { return RendererAPIType::OpenGL; }
}// namespace Engine

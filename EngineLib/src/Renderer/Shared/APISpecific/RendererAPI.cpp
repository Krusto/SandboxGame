#include "RendererAPI.hpp"
#include <Renderer/Renderer.hpp>
#include <Application/ApplicationSpec.hpp>

namespace Engine
{

    void RendererAPI::ClearColor(glm::vec4 color)
    {
        Renderer::RendererAPISetClearColor(m_Data, color.r, color.g, color.b, color.a);
    }

    void RendererAPI::Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec)
    {
        Renderer::RendererAPIInit((void**) &m_Data, &rendererSpec, &applicationSpec);
    }

    void RendererAPI::Shutdown() { Renderer::RendererAPIDestroy((void**) &m_Data); }

    void RendererAPI::BeginFrame() {}

    void RendererAPI::EndFrame() {}

    void RendererAPI::SwitchMode(uint32_t mode) { Renderer::RendererAPISwitchPolygonMode(m_Data, mode); }

    void RendererAPI::SetViewport(ViewportSize size)
    {
        Renderer::RendererAPISetViewport(m_Data, size.width, size.height);
    }

    void RendererAPI::BindDefaultFramebuffer() const { Renderer::RendererAPIBindDefaultFramebuffer(m_Data); }

    void RendererAPI::RenderIndexed(VertexArray vertexArray, uint32_t indexCount) const
    {
        vertexArray.Bind();
        Renderer::RendererAPIRenderIndexed(m_Data, indexCount);
    };

    void RendererAPI::ChangeDepthFunction(DepthFunction depthFunction)
    {
        Renderer::RendererAPIChangeDepthFunc(m_Data, depthFunction);
    }

    void RendererAPI::InitImGUI(GLFWwindow* window) { Renderer::RendererAPIInitIMGUI(m_Data, window); }

    void RendererAPI::DestroyImGUI() { Renderer::RendererAPIDestroyIMGUI(m_Data); }

    void RendererAPI::ImGuiNewFrame() { Renderer::RendererAPIIMGUIBegin(m_Data); }

    void RendererAPI::ImGuiRender(ImDrawData* drawData) { Renderer::RendererAPIIMGUIEnd(m_Data, drawData); }

    RendererAPIType RendererAPI::GetAPI() const { return RendererAPIType::OpenGL; }
}// namespace Engine

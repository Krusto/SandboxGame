#include <Renderer/Shared/APISpecific/RendererAPI.hpp>
#include <Renderer/Shared/APISpecific/GUIContext.hpp>
#include <Renderer/Shared/RendererSpec.hpp>
#include <Application/ApplicationSpec.hpp>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Engine
{
    struct RendererAPIData {
        GUIContext guiContext;
    };

    void RendererAPI::ClearColor(glm::vec4 color)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RendererAPI::Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec)
    {
        if (m_Data == nullptr) { return; }
        m_Data = Allocator::Allocate<RendererAPIData>();
    }

    void RendererAPI::Shutdown()
    {
        if (m_Data == nullptr) { return; }
        m_Data->guiContext.Shutdown();
        Allocator::Deallocate(m_Data);
        m_Data = nullptr;
    };

    void RendererAPI::BeginFrame(){};
    void RendererAPI::EndFrame(){};

    void RendererAPI::SwitchMode(uint32_t mode)
    {
        if (mode == 0) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
        else if (mode == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    }

    void RendererAPI::SetViewport(ViewportSize size) { glViewport(0, 0, size.width, size.height); }

    void RendererAPI::BindDefaultFramebuffer() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void RendererAPI::RenderIndexed(VertexArray vertexArray, uint32_t indexCount) const
    {
        vertexArray.Bind();
        glDrawElements(GL_TRIANGLES, (indexCount == 0) ? vertexArray.IndexCount() : indexCount, GL_UNSIGNED_INT,
                       nullptr);
    };

    void RendererAPI::ChangeDepthFunction(DepthFunction depthFunction)
    {
        switch (depthFunction)
        {
            case DepthFunction::Never:
                glDepthFunc(GL_NEVER);
                break;
            case DepthFunction::Less:
                glDepthFunc(GL_LESS);
                break;
            case DepthFunction::Equal:
                glDepthFunc(GL_EQUAL);
                break;
            case DepthFunction::LEqual:
                glDepthFunc(GL_LEQUAL);
                break;
            case DepthFunction::Greater:
                glDepthFunc(GL_GREATER);
                break;
            case DepthFunction::NotEqual:
                glDepthFunc(GL_NOTEQUAL);
                break;
            case DepthFunction::GEqual:
                glDepthFunc(GL_GEQUAL);
                break;
            case DepthFunction::Always:
                glDepthFunc(GL_ALWAYS);
                break;
        }
    }

    void RendererAPI::InitImGUI(GLFWwindow* window) { m_Data->guiContext.Init(window); }

    void RendererAPI::DestroyImGUI() { m_Data->guiContext.Shutdown(); }

    void RendererAPI::ImGuiNewFrame() { m_Data->guiContext.NewFrame(); }

    void RendererAPI::ImGuiRender(ImDrawData* drawData) { m_Data->guiContext.Render(drawData); }

    RendererAPIType RendererAPI::GetAPI() const { return RendererAPIType::OpenGL; }
}// namespace Engine

#include "GUILayer.hpp"
#include <algorithm>
#include <imgui.h>

GUILayer::GUILayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "GUILayer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    m_AssetsDirectory = m_AppSpec.WorkingDirectory.append("Assets");
    m_ShadersDirectory = (m_AssetsDirectory.string() + "/Shaders");
    m_TexturesDirectory = (m_AssetsDirectory.string() + "/Textures");
}

void GUILayer::Init(Engine::Window* window)
{
    m_Window = window;
    Engine::Renderer::InitImGUI(window->GetRawHandler());
}

void GUILayer::OnAttach() {}

void GUILayer::OnDetach() {}

void GUILayer::Destroy() { }

void GUILayer::OnUpdate(double dt) { m_DeltaTime = dt; }

void GUILayer::OnFixedUpdate(double dt) {}

void GUILayer::OnWindowResizeEvent(int width, int height) {}

void GUILayer::OnImGuiBegin()
{
    Engine::Renderer::ImGuiNewFrame();
    bool p_open = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else { dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode; }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Sandbox", &p_open, window_flags);
    if (!opt_padding) ImGui::PopStyleVar();

    if (opt_fullscreen) ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
}

void GUILayer::OnImGuiDraw() {}

void GUILayer::OnImGuiEnd()
{
    ImGui::End();
    ImGui::Render();

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = m_Window->GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        m_Window->SetCurrentContext(backup_current_context);
    }

    Engine::Renderer::ImGuiRender(ImGui::GetDrawData());
}

void GUILayer::OnMouseMoveEvent(int width, int height) {}

void GUILayer::OnMouseScrollEvent(double x, double y) {}

void GUILayer::OnKeyboardEvent(int action, int key) {}
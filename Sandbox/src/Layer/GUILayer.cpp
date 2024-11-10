#include "GUILayer.hpp"
#include <algorithm>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GUILayer::GUILayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "GUILayer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    m_AssetsDirectory = m_AppSpec.WorkingDirectory.append("Assets");
    m_ShadersDirectory = (m_AssetsDirectory.string() + "/Shaders");
    m_TexturesDirectory = (m_AssetsDirectory.string() + "/Textures");
}

void GUILayer::Init(Ref<Engine::Window> window)
{


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(
            window->GetRawHandler(),
            true);// Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void GUILayer::OnAttach() {}

void GUILayer::OnDetach() {}

void GUILayer::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUILayer::OnUpdate(double dt) { m_DeltaTime = dt; }

void GUILayer::OnWindowResizeEvent(int width, int height) {}

void GUILayer::OnImGuiBegin()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // ImGui::ShowDemoWindow();// Show demo window! :)
}

void GUILayer::OnImGuiDraw() {}

void GUILayer::OnImGuiEnd()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUILayer::OnMouseMoveEvent(int width, int height) {}

void GUILayer::OnMouseScrollEvent(double x, double y) {}

void GUILayer::OnKeyboardEvent(int action, int key) {}
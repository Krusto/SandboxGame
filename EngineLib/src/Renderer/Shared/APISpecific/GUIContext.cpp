#include "GUIContext.hpp"
#include <Renderer/Renderer.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>

namespace Engine
{
    void GUIContext::Init(GLFWwindow* window)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // IF using Docking Branch

        _RendererSpecificInit(window);
    }

    void GUIContext::Shutdown()
    {
        _RendererSpecificShutdown();
        ImGui::DestroyContext();
    }

    void GUIContext::NewFrame() { _RendererSpecificNewFrame(); }

    void GUIContext::Render(ImDrawData* drawData) { _RendererSpecificRender(drawData); }
}// namespace Engine

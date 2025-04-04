#include "GUIContext.hpp"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>

#include <Renderer/Renderer.hpp>
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

        Renderer::GetInstance()->GUIContextInit(nullptr, window);
    }

    void GUIContext::Shutdown()
    {
        Renderer::GetInstance()->GUIContextDestroy(nullptr);
        ImGui::DestroyContext();
    }

    void GUIContext::NewFrame() { Renderer::GetInstance()->GUIContextBegin(nullptr); }

    void GUIContext::Render(ImDrawData* drawData) { Renderer::GetInstance()->GUIContextEnd(nullptr, drawData); }
}// namespace Engine

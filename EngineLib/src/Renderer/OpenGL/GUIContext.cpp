#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <Renderer/Predefines.hpp>

namespace Engine
{
    EXPORT_RENDERER void GUIContextInit(void** data, void* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;    // IF using Docking Branch

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*) window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    EXPORT_RENDERER void GUIContextDestroy(void** data)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }

    EXPORT_RENDERER void GUIContextBegin(void* data)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    EXPORT_RENDERER void GUIContextEnd(void* data, void* drawData)
    {
        ImGui_ImplOpenGL3_RenderDrawData((ImDrawData*) drawData);
    }
}// namespace Engine
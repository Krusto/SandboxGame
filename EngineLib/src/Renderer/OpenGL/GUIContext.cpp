#include <Renderer/Shared/APISpecific/GUIContext.hpp>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Engine
{
    void GUIContext::_RendererSpecificInit(GLFWwindow* window)
    {
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void GUIContext::_RendererSpecificShutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }

    void GUIContext::_RendererSpecificNewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GUIContext::_RendererSpecificRender(ImDrawData* drawData) { ImGui_ImplOpenGL3_RenderDrawData(drawData); }
}// namespace Engine
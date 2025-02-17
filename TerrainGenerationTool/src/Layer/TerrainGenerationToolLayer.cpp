#include "TerrainGenerationToolLayer.hpp"
#include <algorithm>
#include <imgui.h>
#include <GLFW/glfw3.h>

TerrainGenerationToolLayer::TerrainGenerationToolLayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "TerrainGenerationToolLayer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    m_AssetsDirectory = m_AppSpec.WorkingDirectory.append("Assets");
    m_ShadersDirectory = (m_AssetsDirectory.string() + "/Shaders");
    m_TexturesDirectory = (m_AssetsDirectory.string() + "/Textures/Tiles");
    m_ViewportSize = {spec.width, spec.height};
}

void TerrainGenerationToolLayer::Init(Engine::Window* window)
{
    m_Window = window;
    std::string skyboxShaderPath = m_ShadersDirectory.string() + "/Skybox";
    std::string testShaderPath = m_ShadersDirectory.string() + "/TerrainGenerationTool/TerrainGenerationTool";

    m_Shader = Engine::Shader::Create(testShaderPath);
    m_Framebuffer = Engine::Framebuffer::Create(window->GetSpec()->width, window->GetSpec()->height);
}

void TerrainGenerationToolLayer::OnAttach() {}

void TerrainGenerationToolLayer::OnDetach() {}

void TerrainGenerationToolLayer::Destroy() { m_Framebuffer.Destroy(); }

void TerrainGenerationToolLayer::OnFixedUpdate(double dt) {}

void TerrainGenerationToolLayer::OnUpdate(double dt)
{
    using namespace Engine;
    m_DeltaTime = dt;
    m_PassedTime += 0.01;
    if (m_PassedTime > 100.0) { m_PassedTime = 0; }

    m_Camera.Update(dt, 30.0f, 10.0f);

    Renderer::BeginFrame();
    Renderer::SetViewport(m_ViewportSize);
    Renderer::Submit(m_Framebuffer.BindCommand());
    Renderer::Submit(m_Framebuffer.ClearDepthCommand());
    Renderer::Submit(m_Framebuffer.ClearColorCommand(glm::vec4{0.0, 0.0, 0.0, 1.0}));

    Renderer::BindDefaultFramebuffer();

    Renderer::Flush();
    Renderer::EndFrame();
}

void TerrainGenerationToolLayer::OnWindowResizeEvent(int width, int height) { m_Framebuffer.Resize(width, height); }

void TerrainGenerationToolLayer::OnImGuiBegin() {}

void TerrainGenerationToolLayer::OnImGuiDraw()
{
    ImGui::Begin("debug");
    ImGui::Text("%.3fms %.2ffps", m_DeltaTime, 1000.0f / m_DeltaTime);
    ImGui::Checkbox("Lock", &m_LockCamera);
    ImGui::End();

    ImGui::Begin("Framebuffer");
    ImGui::Image((void*) m_Framebuffer.GetColorAttachmentID(),
                 {(float) m_Framebuffer.width(), (float) m_Framebuffer.height()}, {0, 1}, {1, 0});
    ImGui::End();
}

void TerrainGenerationToolLayer::OnImGuiEnd() {}

void TerrainGenerationToolLayer::OnMouseMoveEvent(int width, int height) {}

void TerrainGenerationToolLayer::OnMouseScrollEvent(double x, double y) {}

void TerrainGenerationToolLayer::OnKeyboardEvent(int action, int key) {}
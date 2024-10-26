#include "SandboxLayer.hpp"

SandboxLayer::SandboxLayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "Sandbox Layer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory).string();
}

void SandboxLayer::Init(std::weak_ptr<Engine::Window> window) { }

void SandboxLayer::OnAttach() {}

void SandboxLayer::OnDetach() {}

void SandboxLayer::Destroy() {}

void SandboxLayer::OnUpdate(float dt)
{

    Engine::Renderer::BeginFrame();

    Engine::Renderer::ClearColor(glm::vec4{0.1, 0.2, 0.4, 1.0});

    Engine::Renderer::Flush();

    Engine::Renderer::EndFrame();
}

void SandboxLayer::OnWindowResizeEvent(int width, int height) {}

void SandboxLayer::OnImGuiDraw() {}

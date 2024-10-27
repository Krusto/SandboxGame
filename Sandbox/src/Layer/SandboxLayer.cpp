#include "SandboxLayer.hpp"

SandboxLayer::SandboxLayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "Sandbox Layer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory).string();
}

void SandboxLayer::Init(Ref<Engine::Window> window)
{
    std::string path = m_AppSpec.WorkingDirectory.string() + "/Assets/Shaders/Light";
    m_Shader = Ref<Engine::Shader>(Engine::Shader::Load(path));
    m_World = std::make_unique<Engine::World>();
    m_World->Init(0);
}

void SandboxLayer::OnAttach() {}

void SandboxLayer::OnDetach() {}

void SandboxLayer::Destroy()
{
    m_World->Destroy();
    m_Shader->Destroy();
}

void SandboxLayer::OnUpdate(float dt)
{

    m_World->OnUpdate(dt);

    Engine::Renderer::BeginFrame();

    Engine::Renderer::ClearColor(glm::vec4{0.1, 0.2, 0.4, 1.0});

    m_World->Draw(m_Shader.Raw());

    Engine::Renderer::Flush();

    Engine::Renderer::EndFrame();
}

void SandboxLayer::OnWindowResizeEvent(int width, int height) {}

void SandboxLayer::OnImGuiDraw() {}

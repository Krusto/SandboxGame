#include "SandboxLayer.hpp"

SandboxLayer::SandboxLayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "Sandbox Layer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory).string();
}

void SandboxLayer::Init(Ref<Engine::Window> window)
{
    std::string path = m_AppSpec.WorkingDirectory.string() + "/Assets/Shaders/World";
    m_Shader = Ref<Engine::Shader>(Engine::Shader::Load(path));
    m_World = std::make_unique<Engine::World>();
    m_World->Init(0);

    m_Camera.Init(Engine::CameraSpec({800, 600}, 45.0f, 0.1f, 1000.0f));
    m_Camera.Move({0.0f, 0.0f, 3.0f});
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
    m_DeltaTime = dt;
    m_World->OnUpdate(dt);

    Engine::Renderer::BeginFrame();

    Engine::Renderer::ClearColor(glm::vec4{0.1, 0.2, 0.4, 1.0});

    m_Camera.Upload(m_Shader.Raw());
    m_World->Draw(m_Shader.Raw());

    Engine::Renderer::Flush();

    Engine::Renderer::EndFrame();
}

void SandboxLayer::OnWindowResizeEvent(int width, int height) {}

void SandboxLayer::OnImGuiDraw() {}

void SandboxLayer::OnMouseMoveEvent(int width, int height)
{
    m_Camera.ProcessMouseMovement(width, height, m_DeltaTime, 10000.0f, false);
    LOG_DEBUG("rotation: %f %f\n", m_Camera.GetRotation().x, m_Camera.GetRotation().y);
}

void SandboxLayer::OnKeyboardEvent(int action, int key)
{
    m_Camera.ProcessKeyboardInput(action, key, m_DeltaTime, 100.0f);
    LOG_DEBUG("position: %f %f %f\n", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
}
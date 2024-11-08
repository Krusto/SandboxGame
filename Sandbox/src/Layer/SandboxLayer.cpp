#include "SandboxLayer.hpp"
#include <algorithm>

SandboxLayer::SandboxLayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "Sandbox Layer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    m_AssetsDirectory = m_AppSpec.WorkingDirectory.append("Assets");
    m_ShadersDirectory = (m_AssetsDirectory.string() + "/Shaders");
    m_TexturesDirectory = (m_AssetsDirectory.string() + "/Textures");
}

void SandboxLayer::Init(Ref<Engine::Window> window)
{
    std::string path = m_ShadersDirectory.string() + "/World";
    m_Shader = Ref<Engine::Shader>(Engine::Shader::Load(path));
    m_World = std::make_unique<Engine::World>();

    Engine::TerrainGenerationSettings settings = {.Seed = 0, .AssetsDirectory = m_AssetsDirectory};
    m_World->Init(settings, m_TexturesDirectory);

    m_Camera.Init(Engine::CameraSpec({m_AppSpec.width, m_AppSpec.height}, 45.0f, 0.1f, 1000.0f));
    m_Camera.Move({0.0f, 35.0f, 3.0f});
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

    Engine::Renderer::ClearColor(glm::vec4{1, 1, 1, 1.0});

    m_Camera.Update(dt);
    m_Camera.Upload(m_Shader.Raw());
    m_World->Draw(m_Shader.Raw());

    Engine::Renderer::Flush();

    Engine::Renderer::EndFrame();

    SetShouldExit(true);
}

void SandboxLayer::OnWindowResizeEvent(int width, int height) {}

void SandboxLayer::OnImGuiDraw() {}

void SandboxLayer::OnMouseMoveEvent(int width, int height)
{
    m_Camera.ProcessMouseMovement(width, height, 10000.0f, true);
}

void SandboxLayer::OnMouseScrollEvent(double x, double y) { m_Camera.ProcessMouseScroll(y, 10000); }

void SandboxLayer::OnKeyboardEvent(int action, int key)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS) { Engine::Renderer::SwitchFillMode(); }
    else if (key == GLFW_KEY_V && action == GLFW_PRESS) { Engine::Renderer::SwitchWireframeMode(); }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) { m_Shader->Reload(); }
    else { m_Camera.ProcessKeyboardInput(action, key, 100.0f); }
}
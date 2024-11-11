#include "SandboxLayer.hpp"
#include <algorithm>
#include <imgui.h>

SandboxLayer::SandboxLayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "Sandbox Layer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    m_AssetsDirectory = m_AppSpec.WorkingDirectory.append("Assets");
    m_ShadersDirectory = (m_AssetsDirectory.string() + "/Shaders");
    m_TexturesDirectory = (m_AssetsDirectory.string() + "/Textures");
    m_SkyboxDirectory = (m_TexturesDirectory.string() + "/Skybox");
}

void SandboxLayer::Init(Ref<Engine::Window> window)
{
    m_Window = window;
    std::string worldShaderPath = m_ShadersDirectory.string() + "/World";
    std::string skyboxShaderPath = m_ShadersDirectory.string() + "/Skybox";

    m_Shader = Ref<Engine::Shader>(Engine::Shader::Load(worldShaderPath));

    std::string skyboxName = "skybox_day";
    std::unordered_map<Engine::CubemapTextureFace, std::string> faces = {
            {Engine::CubemapTextureFace::Right, m_SkyboxDirectory.string() + "/" + skyboxName + "_right.png"},
            {Engine::CubemapTextureFace::Left, m_SkyboxDirectory.string() + "/" + skyboxName + "_left.png"},
            {Engine::CubemapTextureFace::Top, m_SkyboxDirectory.string() + "/" + skyboxName + "_top.png"},
            {Engine::CubemapTextureFace::Bottom, m_SkyboxDirectory.string() + "/" + skyboxName + "_bottom.png"},
            {Engine::CubemapTextureFace::Front, m_SkyboxDirectory.string() + "/" + skyboxName + "_front.png"},
            {Engine::CubemapTextureFace::Back, m_SkyboxDirectory.string() + "/" + skyboxName + "_back.png"},
    };
    m_Skybox = Engine::Skybox::Create(skyboxName, skyboxShaderPath, faces);

    m_World = std::make_unique<Engine::World>();

    Engine::TerrainGenerationSettings settings = {.Seed = 0, .AssetsDirectory = m_AssetsDirectory};
    m_World->Init(settings, m_TexturesDirectory);

    m_Camera.Init(Engine::CameraSpec({m_AppSpec.width, m_AppSpec.height}, 45.0f, 0.1f, 1000.0f));
    m_Camera.Move({0.0f, 35.0f, 3.0f});
    m_Camera.Update(m_DeltaTime, 1, 1);
}

void SandboxLayer::OnAttach() {}

void SandboxLayer::OnDetach() {}

void SandboxLayer::Destroy()
{
    m_World->Destroy();
    m_Shader->Destroy();
}

void SandboxLayer::OnUpdate(double dt)
{
    m_DeltaTime = dt;
    m_World->OnUpdate(dt);
    m_Camera.Update(dt, 10.0f, 10.0f);

    Engine::Renderer::BeginFrame();

    Engine::Renderer::ClearColor(glm::vec4{1, 1, 1, 1.0});

    m_Skybox->Draw(&m_Camera);

    m_Shader->Bind();
    m_Skybox->BindTexture(1);
    m_Camera.Upload(m_Shader.Raw());
    m_World->Draw(m_Shader.Raw());

    Engine::Renderer::Flush();

    Engine::Renderer::EndFrame();

    SetShouldExit(true);
}

void SandboxLayer::OnWindowResizeEvent(int width, int height) {}

void SandboxLayer::OnImGuiBegin() {}

void SandboxLayer::OnImGuiDraw() { ImGui::Text("%.3fms %.2ffps", m_DeltaTime, 1000.0f / m_DeltaTime); }

void SandboxLayer::OnImGuiEnd() {}

void SandboxLayer::OnMouseMoveEvent(int width, int height) { m_Camera.ProcessMouseMovement(width, height, 0.1f, true); }

void SandboxLayer::OnMouseScrollEvent(double x, double y) { m_Camera.ProcessMouseScroll(y, 0.1); }

void SandboxLayer::OnKeyboardEvent(int action, int key)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS) { Engine::Renderer::SwitchFillMode(); }
    else if (key == GLFW_KEY_V && action == GLFW_PRESS) { Engine::Renderer::SwitchWireframeMode(); }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        m_Shader->Reload();
        m_Skybox->Reload();
    }
    else
    {
        m_Camera.ProcessKeyboardInput(action, key, glfwGetKey(m_Window->GetRawHandler(), GLFW_KEY_SPACE) == GLFW_PRESS,
                                      glfwGetKey(m_Window->GetRawHandler(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    }
}
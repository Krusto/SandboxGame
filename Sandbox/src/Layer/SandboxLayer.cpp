#include "SandboxLayer.hpp"
#include <Scene/World/ChunkConstraints.hpp>
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

    Engine::TerrainGenerationSettings settings = {.Seed = 0,
                                                  .AssetsDirectory = m_AssetsDirectory,
                                                  .GenerationDistance = 50};
    m_World->Init(settings, m_TexturesDirectory);

    m_Camera.Init(Engine::CameraSpec({m_AppSpec.width, m_AppSpec.height}, 45.0f, 0.1f, 1000.0f));

    glm::vec3 offset{(settings.GenerationDistance / 2) * 32, 0, (settings.GenerationDistance / 2) * 32};
    glm::vec3 startPosition = glm::vec3(0, 150, 3);

    m_Camera.Move(startPosition + offset);
    m_Camera.Update(m_DeltaTime, 1, 1);

    std::string cubeShaderPath = m_ShadersDirectory.string() + "/Cube";

    m_CubeShader = Ref<Engine::Shader>(Engine::Shader::Load(cubeShaderPath));
}

void SandboxLayer::OnAttach() {}

void SandboxLayer::OnDetach() {}

void SandboxLayer::Destroy()
{
    m_World->Destroy();
    m_Shader->Destroy();
    m_CubeShader->Destroy();
    m_Skybox->Destroy();
    Deallocate(m_Skybox);
    for (int i = 0; i < m_Cubes.size(); i++) { Deallocate(m_Cubes[i]); }
    m_Cubes.clear();
}

void SandboxLayer::OnUpdate(double dt)
{
    m_DeltaTime = dt;

    //m_Cube.position = m_Camera.GetPosition();

    // float maxDistance = 10;

    // glm::vec3 rot = glm::radians(m_Camera.GetRotation());

    // glm::vec3 dir(-cos(rot.y), -sin(rot.x), -sin(rot.y));

    // glm::vec3 _pos1 = m_Camera.GetPosition();
    // glm::vec3 _pos2 = _pos1 + dir * glm::vec1{maxDistance};

    // glm::ivec3 pos = _pos1;
    // glm::ivec3 end = _pos2;

    // glm::ivec3 d = glm::ivec3(((_pos1.x < _pos2.x) ? 1 : ((_pos1.x > _pos2.x) ? -1 : 0)),
    //                           ((_pos1.y < _pos2.y) ? 1 : ((_pos1.y > _pos2.y) ? -1 : 0)),
    //                           ((_pos1.z < _pos2.z) ? 1 : ((_pos1.z > _pos2.z) ? -1 : 0)));

    // glm::vec3 deltat = glm::vec3(1.0f / glm::abs(_pos2.x - _pos1.x), 1.0f / glm::abs(_pos2.y - _pos1.y),
    //                              1.0f / glm::abs(_pos2.z - _pos1.z));

    // glm::vec3 min = pos;
    // glm::vec3 max = min + glm::vec3(1.f, 1.f, 1.f);

    // glm::vec3 t = glm::vec3(((_pos1.x > _pos2.x) ? (_pos1.x - min.x) : (max.x - _pos1.x)) * deltat.x,
    //                         ((_pos1.y > _pos2.y) ? (_pos1.y - min.y) : (max.y - _pos1.y)) * deltat.y,
    //                         ((_pos1.z > _pos2.z) ? (_pos1.z - min.z) : (max.z - _pos1.z)) * deltat.z);

    // glm::ivec3 normal = glm::ivec3(0, 0, 0);
    // uint32_t axis{};
    // double count = 0;
    // glm::ivec3 outPosition{};
    // uint8_t outBlock{};
    // glm::ivec3 outNormal{};
    // while ((count++) < maxDistance)
    // {
    //Allocate(Cube, ptr);
    //m_Cubes.emplace_back(ptr);
    //m_Cubes.back()->Init();
    //m_Cubes.back()->position = pos;
    // uint8_t currentBlock = m_World->GetBlock(pos);
    // if (currentBlock != Engine::BlockType::AIR)
    // {
    //     outPosition = pos;
    //     outBlock = currentBlock;
    //     outNormal = normal;
    // LOG("BlockPos: %d %d %d  Block: %d   Normal: %d %d %d\n", outPosition.x, outPosition.y, outPosition.z,
    // outBlock, outNormal.x, outNormal.y, outNormal.z);
    //         break;
    //     }

    //     if (t.x <= t.y && t.x <= t.z)
    //     {
    //         if (pos.x == end.x) break;
    //         t.x += deltat.x;
    //         pos.x += d.x;
    //         axis = 2;
    //         normal = glm::ivec3(-d.x, 0, 0);
    //     }
    //     else if (t.y <= t.z)
    //     {
    //         if (pos.y == end.y) break;
    //         t.y += deltat.y;
    //         pos.y += d.y;
    //         axis = 0;
    //         normal = glm::ivec3(0, -d.y, 0);
    //     }
    //     else
    //     {
    //         if (pos.z == end.z) break;
    //         t.z += deltat.z;
    //         pos.z += d.z;
    //         axis = 4;
    //         normal = glm::ivec3(0, 0, -d.z);
    //     }
    // }


    m_World->OnUpdate(dt);
    m_Camera.Update(dt, 10.0f, 10.0f);

    Engine::Renderer::BeginFrame();

    Engine::Renderer::ClearColor(glm::vec4{1, 1, 1, 1.0});

    m_Skybox->Draw(&m_Camera);
    m_Shader->Bind();
    m_Skybox->BindTexture(1);
    m_Camera.Upload(m_Shader.Raw());
    m_World->Draw(m_Shader.Raw());

    //for (int i = 0; i < m_Cubes.size(); i++) {
    //m_Cubes[i]->Draw(m_CubeShader.Raw(), &m_Camera);
    //}
    //m_Cubes.back()->position += glm::vec3(0.5, 0.5, 0.5);
    //m_Cubes.back()->Draw(axis, m_CubeShader.Raw(), &m_Camera);
    Engine::Renderer::Flush();

    // for (int i = 0; i < m_Cubes.size(); i++) { Deallocate(m_Cubes[i]); }
    // m_Cubes.clear();
    Engine::Renderer::EndFrame();

    if (shouldReloadWorld)
    {
        shouldReloadWorld = false;
        m_World->Reload();
    }
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
    else if (key == GLFW_KEY_T && action == GLFW_PRESS) { shouldReloadWorld = true; }
    else
    {
        m_Camera.ProcessKeyboardInput(action, key, glfwGetKey(m_Window->GetRawHandler(), GLFW_KEY_SPACE) == GLFW_PRESS,
                                      glfwGetKey(m_Window->GetRawHandler(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    }
}
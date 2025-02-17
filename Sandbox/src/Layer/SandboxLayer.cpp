#include "SandboxLayer.hpp"
#include <Scene/World/ChunkConstraints.hpp>
#include <Scene/World/ChunkFactory.hpp>
#include <algorithm>
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <Core/STL/Containers.hpp>

SandboxLayer::SandboxLayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "Sandbox Layer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    m_AssetsDirectory = m_AppSpec.WorkingDirectory.append("Assets");
    m_ShadersDirectory = (m_AssetsDirectory.string() + "/Shaders/Sandbox");
    m_TexturesDirectory = (m_AssetsDirectory.string() + "/Textures/Tiles");
    m_SkyboxDirectory = (m_AssetsDirectory.string() + "/Textures/Skybox");
    m_ViewportSize = {spec.width, spec.height};
}

void SandboxLayer::Init(Engine::Window* window)
{
    m_Window = window;
    std::string worldShaderPath = m_ShadersDirectory.string() + "/World";
    std::string skyboxShaderPath = m_ShadersDirectory.string() + "/Skybox";
    std::string lightShaderPath = m_ShadersDirectory.string() + "/Light";
    std::string depthShaderPath = m_ShadersDirectory.string() + "/WorldDepth";
    std::string hitboxShaderPath = m_ShadersDirectory.string() + "/Hitbox";

    m_Framebuffer = Engine::Framebuffer::Create(window->GetSpec()->width, window->GetSpec()->height);

    m_Shader = Engine::Shader::Create(worldShaderPath);
    m_DepthBufferShader = Engine::Shader::Create(depthShaderPath);
    {
        std::string skyboxName = "skybox_day";
        std::string skyboxPath = m_SkyboxDirectory.string() + "/" + skyboxName;
        std::string skyboxPath_right = skyboxPath + "_right.png";
        std::string skyboxPath_left = skyboxPath + "_left.png";
        std::string skyboxPath_top = skyboxPath + "_top.png";
        std::string skyboxPath_bottom = skyboxPath + "_bottom.png";
        std::string skyboxPath_front = skyboxPath + "_front.png";
        std::string skyboxPath_back = skyboxPath + "_back.png";
        using Engine::CubemapTextureFace;
        using Engine::Pair;
        using Engine::Vector;
        Vector<Pair<CubemapTextureFace, const char*>> faces(
                {Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Right, skyboxPath_right.c_str()),
                 Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Left, skyboxPath_left.c_str()),
                 Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Top, skyboxPath_top.c_str()),
                 Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Bottom, skyboxPath_bottom.c_str()),
                 Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Front, skyboxPath_front.c_str()),
                 Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Back, skyboxPath_back.c_str())});
        m_Skybox = Engine::Skybox::Create(skyboxName, skyboxShaderPath, &faces);
    }

    Engine::TerrainGenerationSettings settings = {.Seed = 0,
                                                  .AssetsDirectory = m_AssetsDirectory,
                                                  .GenerationDistance = 3};
    m_World.Init(settings, m_TexturesDirectory);

    m_Camera.Init(Engine::CameraSpec({m_AppSpec.width, m_AppSpec.height}, 45.0f, 0.1f, 10000.0f));

    glm::vec3 offset{(settings.GenerationDistance / 2) * 32, 0, (settings.GenerationDistance / 2) * 32};
    glm::vec3 startPosition = glm::vec3(0, 150, 3);
    m_Camera.Move(startPosition + offset);
    m_Camera.Update(m_DeltaTime, 1, 1);
    std::string cubeShaderPath = m_ShadersDirectory.string() + "/Cube";
    m_CubeShader = Engine::Shader::Create(cubeShaderPath);
    m_LightShader = Engine::Shader::Create(lightShaderPath);

    m_Light = Engine::Allocator::Allocate<LightObject>();
    m_Light->Init();
    m_Light->position = glm::vec3(0, 0, 0);
    m_Light->rotation = glm::vec3(220, 70, 0);
    m_Light->ambient = glm::vec3(1.0, 0.73, 0.0);
    m_Light->diffuse = glm::vec3(0.78, 0.63, 0.16);

    m_DepthBufferVA = Engine::VertexArray::Create(6);
    float vertices[] = {
            -1, 1,  0.0,//
            -1, -1, 0.0,//
            1,  -1, 0.0,//
            1,  1,  0.0,//
    };
    uint32_t length = sizeof(vertices) / sizeof(float);
    Engine::VertexLayout layout = Engine::CreateVertexLayout(Engine::VertexAttributeList{
            {Engine::VertexAttribute{"Position", (uint8_t) Engine::ShaderUniformType::Vec3}},
            1});

    m_DepthBufferVA.AddVertexBuffer(&layout, vertices, length);

    uint32_t indices[] = {0, 1, 2, 0, 2, 3};
    m_DepthBufferVA.AddIndexBuffer(indices, 6);

    m_DebugShader = Engine::Shader::Create(m_ShadersDirectory.string() + "/Debug");


    glm::ivec3 currentChunkPos = glm::ivec3(0, 0, 0);
    glm::ivec3 playerChunkPos =
            glm::ivec3(m_Camera.GetPosition().x / Engine::CHUNK_SIZE, 0, m_Camera.GetPosition().z / Engine::CHUNK_SIZE);
    uint32_t worldSize = settings.GenerationDistance;

    uint32_t maxChunksY = settings.maxTerrainHeight / Engine::CHUNK_SIZE;

    for (int z = 0; z < worldSize * 2; z++)
    {
        for (int x = 0; x < worldSize * 2; x++)
        {
            //if (z * z + x * x > worldSize * worldSize) { continue; }
            for (int y = 0; y < maxChunksY; y++)
            {
                currentChunkPos = glm::ivec3(x, y, z) + playerChunkPos;
                Engine::ChunkFactory::ScheduleChunkForGeneration(currentChunkPos);
            }
        }
    }

    m_HitboxShader = Engine::Shader::Create(hitboxShaderPath);
    m_DebugCube = Engine::Allocator::Allocate<Hitbox>();
    m_DebugCube->Init();
}

void SandboxLayer::OnAttach() {}

void SandboxLayer::OnDetach() {}

void SandboxLayer::Destroy()
{
    m_World.Destroy();
    m_Shader.Destroy();
    m_DepthBufferShader.Destroy();
    m_CubeShader.Destroy();
    m_LightShader.Destroy();
    m_DebugShader.Destroy();
    m_HitboxShader.Destroy();

    m_Skybox.Destroy();

    Engine::Allocator::Deallocate(m_Light);
    m_Light = nullptr;

    m_Framebuffer.Destroy();

    m_DepthBufferVA.Destroy();

    m_DebugCube->Destroy();
    Engine::Allocator::Deallocate(m_DebugCube);
    m_DebugCube = nullptr;
}

void SandboxLayer::RenderWorld()
{
    using namespace Engine;
    Renderer::SetViewport(m_ViewportSize);
    Renderer::Submit(m_Framebuffer.BindCommand());
    Renderer::Submit(m_Framebuffer.ClearDepthCommand());
    Renderer::Submit(m_Framebuffer.ClearColorCommand(glm::vec4{0.0, 0.0, 0.0, 1.0}));

    //Render Skybox
    Renderer::Submit(m_Skybox.RenderCommand(&m_Camera));

    //Render World
    Renderer::Submit(m_Shader.BindCommand());
    Renderer::Submit(m_Skybox.BindTexture(1));

    Renderer::Submit(m_Camera.UploadCommand(&m_Shader));

    Renderer::Submit(m_Light->UploadLight(&m_Shader));
    Renderer::Submit(RendererCommand([=]() { m_Shader.SetUniform("light.shininess", (float) m_WorldShininess); }));

    Renderer::Submit(m_World.RenderWorldCommand(&m_Shader, m_Camera.GetPosition()));

    // Render light debug object
    Renderer::Submit(m_LightShader.BindCommand());
    Renderer::Submit(m_Camera.UploadCommand(&m_LightShader));
    Renderer::Submit(m_Light->Render(&m_LightShader));
}

void SandboxLayer::RenderDepthWorld()
{
    Engine::Renderer::SetViewport(m_ViewportSize);
    Engine::Renderer::Submit(m_DepthFramebuffer.BindCommand());
    Engine::Renderer::Submit(m_DepthFramebuffer.ClearDepthCommand());
    //Render World
    float near_plane = 1.0f, far_plane = 1000.0f;

    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView =
            glm::lookAt(m_Light->position, glm::radians(m_Camera.GetPosition()), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 lightMatrix = lightProjection * lightView;
    Engine::Renderer::Submit(Engine::RendererCommand([=]() {
        m_DepthBufferShader.Bind();
        m_DepthBufferShader.SetUniform("lightSpaceMatrix", lightMatrix);
    }));

    Engine::Renderer::Submit(m_World.RenderWorldCommand(&m_DepthBufferShader, m_Light->position));

    Engine::Renderer::Submit(Engine::RendererCommand([=]() { m_DebugFramebuffer.Bind(); }));
    Engine::Renderer::SetViewport({m_DebugFramebuffer.width(), m_DebugFramebuffer.height()});
    Engine::Renderer::Submit(m_DebugFramebuffer.ClearDepthCommand());
    Engine::Renderer::Submit(m_DebugFramebuffer.ClearColorCommand(glm::vec4{0.0, 0.0, 0.0, 1.0}));
    Engine::Renderer::Submit(m_DebugShader.BindCommand());
    Engine::Renderer::Submit(m_DepthFramebuffer.BindDepthTextureCommand(0));
    Engine::Renderer::SubmitRenderIndexed(m_DepthBufferVA, 6);
    Engine::Renderer::BindDefaultFramebuffer();
}

void SandboxLayer::OnFixedUpdate(double dt) {}

void SandboxLayer::OnUpdate(double dt)
{
    m_DeltaTime = dt;
    m_PassedTime += 0.01;
    if (m_PassedTime > 100.0) { m_PassedTime = 0; }
    m_World.OnUpdate(dt);

    if (!m_DisableGravity)
    {
        if (m_World.GetBlock(m_Camera.GetPosition() - glm::vec3(0, 1.75, 0)) == Engine::BlockType::AIR)
        {
            if (m_World.GetBlock(m_Camera.GetPosition() - glm::vec3(0, 1.75, 0) - glm::vec3(0, 1 / velocity, 0)) ==
                Engine::BlockType::AIR)
            {
                m_Camera.Move(glm::vec3(0, -velocity, 0));
                velocity += (1000 / ((m_PassedTime / 100) * dt));
                if (velocity > 0.6) { velocity = 0.6; }
            }
            else { velocity = 0; }
        }
    }
    m_Camera.Update(dt, 30.0f, 10.0f);
    m_Skybox.Update(dt);

    Engine::Renderer::BeginFrame();

    RenderWorld();


    {
        //m_Cube.position = m_Camera.GetPosition();

        float maxDistance = 40.0f;

        glm::vec3 rot = glm::radians(m_Camera.GetRotation());

        glm::vec3 dir =
                glm::vec3(glm::cos(rot.x) * (-glm::cos(rot.y)), -glm::sin(rot.x), glm::cos(rot.x) * (-glm::sin(rot.y)));

        glm::vec3 _pos1 = m_Camera.GetPosition() + glm::vec3(0.5, 1.5, 0.5);
        glm::vec3 _pos2 = _pos1 + dir * glm::vec1{maxDistance};
        glm::ivec3 pos = _pos1;
        glm::ivec3 end = _pos2;

        glm::ivec3 d = glm::ivec3(((_pos1.x < _pos2.x) ? 1 : ((_pos1.x > _pos2.x) ? -1 : 0)),
                                  ((_pos1.y < _pos2.y) ? 1 : ((_pos1.y > _pos2.y) ? -1 : 0)),
                                  ((_pos1.z < _pos2.z) ? 1 : ((_pos1.z > _pos2.z) ? -1 : 0)));

        glm::vec3 deltat = glm::vec3(1.0f / glm::abs(_pos2.x - _pos1.x), 1.0f / glm::abs(_pos2.y - _pos1.y),
                                     1.0f / glm::abs(_pos2.z - _pos1.z));

        glm::vec3 min = pos;
        glm::vec3 max = min + glm::vec3(1.f, 1.f, 1.f);

        glm::vec3 t = glm::vec3(((_pos1.x > _pos2.x) ? (_pos1.x - min.x) : (max.x - _pos1.x)) * deltat.x,
                                ((_pos1.y > _pos2.y) ? (_pos1.y - min.y) : (max.y - _pos1.y)) * deltat.y,
                                ((_pos1.z > _pos2.z) ? (_pos1.z - min.z) : (max.z - _pos1.z)) * deltat.z);

        glm::ivec3 normal = glm::ivec3(0, 0, 0);
        uint32_t axis{};
        double count = 0;
        glm::ivec3 outPosition{};
        uint8_t outBlock{};
        glm::ivec3 outNormal{};
        while ((count++) < maxDistance)
        {
            uint8_t currentBlock = m_World.GetBlock(pos);
            if (currentBlock != Engine::BlockType::AIR)
            {
                outPosition = pos;
                outBlock = currentBlock;
                outNormal = normal;
                //LOG("BlockPos: %d %d %d  Block: %d  Axis %d Normal: %d %d %d\n", outPosition.x, outPosition.y,
                //outPosition.z, outBlock, axis, outNormal.x, outNormal.y, outNormal.z);
                break;
            }

            if (t.y <= t.z)
            {
                if (pos.y == end.y) break;
                t.y += deltat.y;
                pos.y += d.y;
                axis = 0;
                normal = glm::ivec3(0, -d.y, 0);
                if (normal.y > 0) { axis = 1; }
            }
            else if (t.x <= t.y && t.x <= t.z)
            {
                if (pos.x == end.x) break;
                t.x += deltat.x;
                pos.x += d.x;
                axis = 2;
                normal = glm::ivec3(-d.x, 0, 0);
                if (normal.x > 0) { axis = 3; }
            }
            else
            {
                if (pos.z == end.z) { break; };
                t.z += deltat.z;
                pos.z += d.z;
                axis = 4;
                normal = glm::ivec3(0, 0, -d.z);
                if (normal.z > 0) { axis = 5; }
            }
        }
        if (outPosition != (glm::ivec3) m_DebugCube->position)
        {
            m_LookingAt = outPosition;
            m_DebugCube->position = outPosition;
        }

        //Render debug cube object
        Engine::Renderer::Submit(m_HitboxShader.BindCommand());
        Engine::Renderer::Submit(m_Camera.UploadCommand(&m_HitboxShader));
        Engine::Renderer::Submit(m_DebugCube->Render(&m_HitboxShader, m_PassedTime, axis));
    }

    Engine::Renderer::BindDefaultFramebuffer();

    Engine::Renderer::Flush();
    Engine::Renderer::EndFrame();

    if (shouldReloadWorld)
    {
        shouldReloadWorld = false;
        m_World.Reload();
    }
}

void SandboxLayer::OnWindowResizeEvent(int width, int height) { m_Framebuffer.Resize(width, height); }

void SandboxLayer::OnImGuiBegin() {}

void SandboxLayer::OnImGuiDraw()
{
    ImGui::Begin("debug");
    ImGui::Text("%.3fms %.2ffps", m_DeltaTime, 1000.0f / m_DeltaTime);
    ImGui::SliderFloat3("Light Position", (float*) &m_Light->position, 0, 360);
    ImGui::SliderFloat3("Light Rotation", (float*) &m_Light->rotation, 0, 360);
    ImGui::ColorEdit3("Ambient Color", (float*) &m_Light->ambient);
    ImGui::ColorEdit3("Diffuse Color", (float*) &m_Light->diffuse);
    ImGui::ColorEdit3("Specular Color", (float*) &m_Light->specular);
    ImGui::SliderInt("Shininess", &m_WorldShininess, 0, 100);
    ImGui::SliderFloat("Intensity", &m_Light->intensity, 0.0, 10.0);
    ImGui::Checkbox("Disable Gravity", &m_DisableGravity);
    ImGui::Checkbox("Lock", &m_LockCamera);
    ImGui::End();


    ImGui::Begin("Framebuffer");
    if (m_ShowDepthBuffer)
    {
        ImGui::Image((void*) m_DebugFramebuffer.GetColorAttachmentID(),
                     {(float) m_DebugFramebuffer.width(), (float) m_DebugFramebuffer.height()}, {0, 1}, {1, 0});
    }
    else
    {
        ImGui::Image((void*) m_Framebuffer.GetColorAttachmentID(),
                     {(float) m_Framebuffer.width(), (float) m_Framebuffer.height()}, {0, 1}, {1, 0});
    }
    ImGui::End();
}

void SandboxLayer::OnImGuiEnd() {}

void SandboxLayer::OnMouseMoveEvent(int width, int height)
{
    if (m_LockCamera == false) { m_Camera.ProcessMouseMovement(width, height, 0.1f, true); }
}

void SandboxLayer::OnMouseButtonEvent(int action, int button)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS) { m_World.RemoveBlock(m_LookingAt); }
            break;
    }
}

void SandboxLayer::OnMouseScrollEvent(double x, double y)
{
    if (m_LockCamera == false) { m_Camera.ProcessMouseScroll(y, 0.1); }
}

void SandboxLayer::OnKeyboardEvent(int action, int key)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) { m_LockCamera = !m_LockCamera; }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) { Engine::Renderer::SwitchFillMode(); }
    else if (key == GLFW_KEY_V && action == GLFW_PRESS) { Engine::Renderer::SwitchWireframeMode(); }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        m_Shader.Reload();
        m_DepthBufferShader.Reload();
        m_CubeShader.Reload();
        m_Skybox.Reload();
        m_LightShader.Reload();
        m_DebugShader.Reload();
        m_HitboxShader.Reload();
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS) { m_Light->position = m_Camera.GetPosition(); }
    else if (key == GLFW_KEY_O && action == GLFW_PRESS) { m_DebugCube->position = m_Camera.GetPosition(); }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS) { shouldReloadWorld = true; }
    else
    {
        if (m_LockCamera == false)
        {
            m_Camera.ProcessKeyboardInput(action, key,
                                          glfwGetKey(m_Window->GetRawHandler(), GLFW_KEY_SPACE) == GLFW_PRESS,
                                          glfwGetKey(m_Window->GetRawHandler(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
        }
    }
}
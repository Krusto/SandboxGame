#include "SandboxLayer.hpp"
#include <Scene/World/ChunkConstraints.hpp>
#include <algorithm>
#include <imgui.h>
#include <glad/glad.h>

SandboxLayer::SandboxLayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "Sandbox Layer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    m_AssetsDirectory = m_AppSpec.WorkingDirectory.append("Assets");
    m_ShadersDirectory = (m_AssetsDirectory.string() + "/Shaders");
    m_TexturesDirectory = (m_AssetsDirectory.string() + "/Textures");
    m_SkyboxDirectory = (m_TexturesDirectory.string() + "/Skybox");
    m_ViewportSize = {spec.width, spec.height};
}

void SandboxLayer::Init(Engine::Window* window)
{
    m_Window = window;
    std::string worldShaderPath = m_ShadersDirectory.string() + "/World";
    std::string skyboxShaderPath = m_ShadersDirectory.string() + "/Skybox";
    std::string lightShaderPath = m_ShadersDirectory.string() + "/Light";
    std::string depthShaderPath = m_ShadersDirectory.string() + "/WorldDepth";

    m_Framebuffer = Engine::Framebuffer::Create(window->GetSpec()->width, window->GetSpec()->height);
    m_DepthFramebuffer = Engine::Framebuffer::Create(5000, 5000, true);

    m_Shader = Ref<Engine::Shader>(Engine::Shader::Load(worldShaderPath));
    m_DepthBufferShader = Ref<Engine::Shader>(Engine::Shader::Load(depthShaderPath));

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
                                                  .GenerationDistance = 10};
    m_World->Init(settings, m_TexturesDirectory);

    m_Camera.Init(Engine::CameraSpec({m_AppSpec.width, m_AppSpec.height}, 45.0f, 0.1f, 1000.0f));

    glm::vec3 offset{(settings.GenerationDistance / 2) * 32, 0, (settings.GenerationDistance / 2) * 32};
    glm::vec3 startPosition = glm::vec3(0, 150, 3);

    m_Camera.Move(startPosition + offset);
    m_Camera.Update(m_DeltaTime, 1, 1);

    std::string cubeShaderPath = m_ShadersDirectory.string() + "/Cube";

    m_CubeShader = Ref<Engine::Shader>(Engine::Shader::Load(cubeShaderPath));
    m_LightShader = Ref<Engine::Shader>(Engine::Shader::Load(lightShaderPath));

    m_Light = Engine::Allocator::Allocate<LightObject>();
    m_Light->Init();
    m_Light->position = glm::vec3(0, 0, 0);


    m_DepthBufferVA = Engine::VertexArray::Create(6);
    float vertices[] = {
            -1, 1,  0.0, 0, 1,//
            -1, -1, 0.0, 0, 0,//
            1,  -1, 0.0, 1, 0,//
            1,  1,  0.0, 1, 1,//
    };
    uint32_t length = sizeof(vertices) / sizeof(float);
    Engine::VertexLayout layout =
            Engine::VertexLayout({Engine::VertexAttribute{"Position", Engine::ShaderUniformType::Vec3},
                                  Engine::VertexAttribute{"TextureCoord", Engine::ShaderUniformType::Vec2}});

    m_DepthBufferVA->AddVertexBuffer(layout, vertices, length);

    uint32_t indices[] = {0, 1, 2, 0, 2, 3};
    m_DepthBufferVA->AddIndexBuffer(indices, 6);

    m_DebugShader = Ref<Engine::Shader>(Engine::Shader::Load(m_ShadersDirectory.string() + "/Debug"));
}

void SandboxLayer::OnAttach() {}

void SandboxLayer::OnDetach() {}

void SandboxLayer::Destroy()
{
    m_World->Destroy();
    m_Shader->Destroy();
    m_DepthBufferShader->Destroy();
    m_CubeShader->Destroy();
    m_LightShader->Destroy();
    m_Skybox->Destroy();
    Engine::Allocator::Deallocate(m_Skybox);

    Engine::Allocator::Deallocate(m_Light);
    m_Framebuffer->Destroy();
    Engine::Allocator::Deallocate(m_Framebuffer);
    m_DepthFramebuffer->Destroy();
    Engine::Allocator::Deallocate(m_DepthFramebuffer);
    // for (int i = 0; i < m_Cubes.size(); i++) { Engine::Allocator::Deallocate(m_Cubes[i]); }
    // m_Cubes.clear();
}

void SandboxLayer::RenderWorld()
{
    Engine::Renderer::Submit(
            Engine::RendererCommand([=]() { glViewport(0, 0, m_ViewportSize.width, m_ViewportSize.height); }));
    Engine::Renderer::Submit(m_Framebuffer->BindCommand());
    Engine::Renderer::ClearColor(glm::vec4{1, 1, 1, 1.0});
    //Render Skybox
    Engine::Renderer::Submit(m_Skybox->RenderCommand(&m_Camera));
    //Render World
    Engine::Renderer::Submit(m_Shader->BindCommand());
    Engine::Renderer::Submit(m_Skybox->BindTexture(1));
    Engine::Renderer::Submit(Engine::RendererCommand([=]() {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_DepthFramebuffer->GetDepthAttachmentID());
    }));
    Engine::Renderer::Submit(m_Camera.UploadCommand(m_Shader.Raw()));

    float near_plane = 1.0f, far_plane = 1000.0f;

    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(m_Light->position, glm::radians(m_Light->rotation), glm::vec3(0.0f, 1.0f, 0.0f));


    glm::mat4 lightMatrix = lightProjection * lightView;
    Engine::Renderer::Submit(Engine::RendererCommand([=]() { m_Shader->SetUniform("lightSpaceMatrix", lightMatrix); }));

    Engine::Renderer::Submit(m_Light->UploadLight(m_Shader.Raw()));
    Engine::Renderer::Submit(
            Engine::RendererCommand([=]() { m_Shader->SetUniform("light.shininess", (float) m_WorldShininess); }));

    Engine::Renderer::Submit(m_World->RenderWorldCommand(m_Shader.Raw()));

    if (!m_DisableLighting)
    {
        //Render light debug object
        Engine::Renderer::Submit(m_LightShader->BindCommand());
        Engine::Renderer::Submit(m_Camera.UploadCommand(m_LightShader.Raw()));
        Engine::Renderer::Submit(m_Light->Render(m_LightShader.Raw()));
    }

    Engine::Renderer::Submit(Engine::RendererCommand([=]() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }));
}

void SandboxLayer::RenderDepthWorld()
{
    Engine::Renderer::Submit(Engine::RendererCommand([=]() { glViewport(0, 0, 1000, 1000); }));
    Engine::Renderer::Submit(m_DepthFramebuffer->BindCommand());
    Engine::Renderer::Submit(Engine::RendererCommand([=]() { glClear(GL_DEPTH_BUFFER_BIT); }));
    //Render World
    float near_plane = 1.0f, far_plane = 1000.0f;

    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(m_Light->position, glm::radians(m_Light->rotation), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 lightMatrix = lightProjection * lightView;
    Engine::Renderer::Submit(Engine::RendererCommand([=]() {
        ;
        m_DepthBufferShader->Bind();
        m_DepthBufferShader->SetUniform("lightSpaceMatrix", lightMatrix);
    }));

    Engine::Renderer::Submit(m_World->RenderWorldCommand(m_DepthBufferShader.Raw()));
    // Engine::Renderer::Submit(Engine::RendererCommand([=]() { m_Framebuffer->Bind(); }));
    // Engine::Renderer::Submit(Engine::RendererCommand([=]() {
    //     glViewport(0, 0, m_ViewportSize.width, m_ViewportSize.height);
    //     glClearColor(0, 0, 0, 1);
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //     m_DebugShader->Bind();
    //     glActiveTexture(GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, m_DepthFramebuffer->GetDepthAttachmentID());
    //     m_DepthBufferVA->Bind();
    //     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    //     glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // }));
}

void SandboxLayer::OnUpdate(double dt)
{
    m_DeltaTime = dt;
    {
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
    }

    m_World->OnUpdate(dt);
    m_Camera.Update(dt, 10.0f, 10.0f);

    Engine::Renderer::BeginFrame();

    RenderDepthWorld();
    RenderWorld();

    Engine::Renderer::Flush();
    Engine::Renderer::EndFrame();

    if (shouldReloadWorld)
    {
        shouldReloadWorld = false;
        m_World->Reload();
    }
}

void SandboxLayer::OnWindowResizeEvent(int width, int height) { m_Framebuffer->Resize(width, height); }

void SandboxLayer::OnImGuiBegin() {}

void SandboxLayer::OnImGuiDraw()
{
    ImGui::Begin("debug");
    ImGui::Text("%.3fms %.2ffps", m_DeltaTime, 1000.0f / m_DeltaTime);
    ImGui::Checkbox("Disable Lighting", &m_DisableLighting);
    // // if (!m_DisableLighting)
    // // {
    ImGui::SliderFloat3("lightPos", (float*) &m_Light->position, 0, 360);
    ImGui::SliderFloat3("lightRot", (float*) &m_Light->rotation, 0, 360);
    ImGui::ColorEdit3("Ambient", (float*) &m_Light->ambient);
    ImGui::ColorEdit3("Diffuse", (float*) &m_Light->diffuse);
    ImGui::ColorEdit3("Specular", (float*) &m_Light->specular);
    ImGui::SliderInt("Shininess", &m_WorldShininess, 0, 100);
    ImGui::SliderFloat("Intensity", &m_Light->intensity, 0.0, 10.0);
    // // }

    ImGui::Checkbox("Lock", &m_LockCamera);
    ImGui::Checkbox("Show Depth Buffer", &m_ShowDepthBuffer);
    ImGui::End();


    ImGui::Begin("Framebuffer");
    if (m_ShowDepthBuffer)
    {
        ImGui::Image((void*) m_DepthFramebuffer->GetDepthAttachmentID(),
                     {(float) m_DepthFramebuffer->width(), (float) m_DepthFramebuffer->height()}, {0, 1}, {1, 0});
    }
    else
    {
        ImGui::Image((void*) m_Framebuffer->GetColorAttachmentID(),
                     {(float) m_Framebuffer->width(), (float) m_Framebuffer->height()}, {0, 1}, {1, 0});
    }
    ImGui::End();
}

void SandboxLayer::OnImGuiEnd() {}

void SandboxLayer::OnMouseMoveEvent(int width, int height)
{
    if (m_LockCamera == false) { m_Camera.ProcessMouseMovement(width, height, 0.1f, true); }
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
        m_Shader->Reload();
        m_DepthBufferShader->Reload();
        m_CubeShader->Reload();
        m_Skybox->Reload();
    }
    else if (key == GLFW_KEY_L && action == GLFW_PRESS) { m_Light->position = m_Camera.GetPosition(); }
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
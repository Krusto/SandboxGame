#include "TestLayer.hpp"
#include <algorithm>
#include <imgui.h>
#include <GLFW/glfw3.h>

TestLayer::TestLayer(const Engine::ApplicationSpec& spec)
{
    m_Name = "Test Layer";
    m_AppSpec = spec;
    m_AppSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    m_AssetsDirectory = m_AppSpec.WorkingDirectory.append("Assets");
    m_ShadersDirectory = (m_AssetsDirectory.string() + "/Shaders");
    m_TexturesDirectory = (m_AssetsDirectory.string() + "/Textures/Tiles");
    m_SkyboxDirectory = (m_AssetsDirectory.string() + "/Textures/Skybox");
    m_ViewportSize = {spec.width, spec.height};
}

void TestLayer::Init(Engine::Window* window)
{
    m_Window = window;
    std::string skyboxShaderPath = m_ShadersDirectory.string() + "/Skybox";
    std::string testShaderPath = m_ShadersDirectory.string() + "/Test";

    m_Shader = Engine::Shader::Create(testShaderPath);
    m_Framebuffer = Engine::Framebuffer::Create(window->GetSpec()->width, window->GetSpec()->height);

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

    m_Camera.Init(Engine::CameraSpec({m_AppSpec.width, m_AppSpec.height}, 45.0f, 0.1f, 1000.0f));
    m_Camera.SetPosition({0.0f, 0.0f, 0.0f});
    m_Camera.Update(m_DeltaTime, 1, 1);

    m_VA = Engine::VertexArray::Create(6);
    float vertices[] = {
            -1, 1,  0.0,//
            -1, -1, 0.0,//
            1,  -1, 0.0,//
            1,  1,  0.0,//
    };
    uint32_t length = sizeof(vertices) / sizeof(float);
    Engine::VertexLayout layout =
            Engine::VertexLayout({Engine::VertexAttribute{"Position", Engine::ShaderUniformType::Vec3}});
    //   Engine::VertexAttribute{"TextureCoord", Engine::ShaderUniformType::Vec2}});

    m_VA.AddVertexBuffer(&layout, vertices, 4);

    uint32_t indices[] = {0, 1, 2, 0, 2, 3};
    m_VA.AddIndexBuffer(indices, 6);
}

void TestLayer::OnAttach() {}

void TestLayer::OnDetach() {}

void TestLayer::Destroy()
{
    m_Skybox.Destroy();

    m_Framebuffer.Destroy();
}

void TestLayer::RenderWorld()
{
    using namespace Engine;
    Renderer::SetViewport(m_ViewportSize);
    Renderer::Submit(m_Framebuffer.BindCommand());
    Renderer::Submit(m_Framebuffer.ClearDepthCommand());
    Renderer::Submit(m_Framebuffer.ClearColorCommand(glm::vec4{0.0, 0.0, 0.0, 1.0}));

    //Render Skybox
    Renderer::Submit(m_Skybox.RenderCommand(&m_Camera));
    // Renderer::Submit(m_Shader.BindCommand());
    // Renderer::SubmitRenderIndexed(m_VA, 6);
}

void TestLayer::OnFixedUpdate(double dt) {}

void TestLayer::OnUpdate(double dt)
{
    m_DeltaTime = dt;
    m_PassedTime += 0.01;
    if (m_PassedTime > 100.0) { m_PassedTime = 0; }

    m_Camera.Update(dt, 30.0f, 10.0f);
    m_Skybox.Update(dt);

    Engine::Renderer::BeginFrame();

    RenderWorld();

    Engine::Renderer::BindDefaultFramebuffer();

    Engine::Renderer::Flush();
    Engine::Renderer::EndFrame();
}

void TestLayer::OnWindowResizeEvent(int width, int height) { m_Framebuffer.Resize(width, height); }

void TestLayer::OnImGuiBegin() {}

void TestLayer::OnImGuiDraw()
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

void TestLayer::OnImGuiEnd() {}

void TestLayer::OnMouseMoveEvent(int width, int height)
{
    if (m_LockCamera == false) { m_Camera.ProcessMouseMovement(width, height, 0.1f, true); }
}

void TestLayer::OnMouseScrollEvent(double x, double y)
{
    if (m_LockCamera == false) { m_Camera.ProcessMouseScroll(y, 0.1); }
}

void TestLayer::OnKeyboardEvent(int action, int key)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) { m_LockCamera = !m_LockCamera; }
    if (key == GLFW_KEY_C && action == GLFW_PRESS) { Engine::Renderer::SwitchFillMode(); }
    else if (key == GLFW_KEY_V && action == GLFW_PRESS) { Engine::Renderer::SwitchWireframeMode(); }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) { m_Skybox.Reload(); }
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
#pragma once
#include <memory>
#include <Engine.hpp>

class TestLayer: public Engine::Layer
{
public:
    TestLayer() = default;
    explicit TestLayer(const Engine::ApplicationSpec& spec);
    ~TestLayer() = default;

public:
    void Init(Engine::Window* window) override;

    void OnAttach() override;

    void OnUpdate(double dt) override;

    void OnFixedUpdate(double dt) override;

    void OnDetach() override;

    void Destroy() override;

    void OnImGuiBegin() override;

    void OnImGuiDraw() override;

    void OnImGuiEnd() override;

    void OnMouseMoveEvent(int width, int height) override;

    void OnKeyboardEvent(int action, int key) override;

    void OnWindowResizeEvent(int width, int height) override;

    void OnMouseScrollEvent(double x, double y) override;

    void OnWindowShouldCloseEvent() override { SetShouldExit(true); }

    void Begin() override {}

    void End() override {}

    std::string_view GetName() override { return m_Name; }

    std::string_view GetName() const override { return m_Name; }

    bool ShouldExit() override { return m_ShouldExit; }

    void SetShouldExit(bool value) override { m_ShouldExit = value; }

    void RenderWorld();

private:
    std::string m_Name;
    bool m_ShouldExit = false;
    Engine::Window* m_Window;

    Engine::Framebuffer m_Framebuffer;
    Engine::Skybox m_Skybox{};

    Engine::Camera m_Camera;

    double m_DeltaTime = 0.0f;
    double m_PassedTime{};

    ViewportSize m_ViewportSize;
    Engine::ApplicationSpec m_AppSpec;
    std::filesystem::path m_AssetsDirectory;
    std::filesystem::path m_ShadersDirectory;
    std::filesystem::path m_TexturesDirectory;
    std::filesystem::path m_SkyboxDirectory;

    Engine::Shader m_Shader;
    Engine::VertexArray m_VA;

    bool m_LockCamera{};
};

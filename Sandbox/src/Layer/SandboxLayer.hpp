#pragma once
#include <memory>
#include <Engine.hpp>
#include "Cube.hpp"

class SandboxLayer: public Engine::Layer
{
public:
    SandboxLayer() = default;
    explicit SandboxLayer(const Engine::ApplicationSpec& spec);
    ~SandboxLayer() = default;

public:
    void Init(Ref<Engine::Window> window) override;

    void OnAttach() override;

    void OnUpdate(double dt) override;

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

protected:
    Ref<Engine::Window> m_Window;
    Ref<Engine::Shader> m_Shader;
    Ref<Engine::Shader> m_CubeShader;
    Engine::Skybox* m_Skybox{};
    std::unique_ptr<Engine::World> m_World;
    bool shouldReloadWorld{};

    Engine::Camera m_Camera;

    double m_DeltaTime = 0.0f;

    ViewportSize m_ViewportSize;
    Engine::ApplicationSpec m_AppSpec;
    std::filesystem::path m_AssetsDirectory;
    std::filesystem::path m_ShadersDirectory;
    std::filesystem::path m_TexturesDirectory;
    std::filesystem::path m_SkyboxDirectory;

    std::vector<Cube*> m_Cubes;
};

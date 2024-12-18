#pragma once
#include <memory>
#include <Engine.hpp>
#include "LightObject.hpp"
#include "DebugCube.hpp"
#include "Hitbox.hpp"

class SandboxLayer: public Engine::Layer
{
public:
    SandboxLayer() = default;
    explicit SandboxLayer(const Engine::ApplicationSpec& spec);
    ~SandboxLayer() = default;

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
    void RenderDepthWorld();

private:
    std::string m_Name;
    bool m_ShouldExit = false;
    Engine::Window* m_Window;
    Engine::Shader m_Shader;
    Engine::Shader m_CubeShader;
    Engine::Shader m_LightShader;
    Engine::Shader m_DepthBufferShader;
    Engine::Shader m_DebugShader;
    Engine::Shader m_HitboxShader;

    Engine::Framebuffer m_Framebuffer;
    Engine::Framebuffer m_DebugFramebuffer;
    Engine::Framebuffer m_DepthFramebuffer;
    Engine::VertexArray m_DepthBufferVA;
    Engine::Skybox m_Skybox{};

    LightObject* m_Light;

    std::unique_ptr<Engine::World> m_World;
    bool shouldReloadWorld{};

    Engine::Camera m_Camera;

    double m_DeltaTime = 0.0f;
    double m_PassedTime{};

    ViewportSize m_ViewportSize;
    Engine::ApplicationSpec m_AppSpec;
    std::filesystem::path m_AssetsDirectory;
    std::filesystem::path m_ShadersDirectory;
    std::filesystem::path m_TexturesDirectory;
    std::filesystem::path m_SkyboxDirectory;

    int m_WorldShininess{30};
    bool m_DisableGravity{true};
    bool m_ShowDepthBuffer{false};
    bool m_LockCamera{};

    double velocity{};
    Hitbox* m_DebugCube;
};

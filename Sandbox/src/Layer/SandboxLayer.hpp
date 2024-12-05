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

    void RenderWorld();
    void RenderDepthWorld();

protected:
    Engine::Window* m_Window;
    Ref<Engine::Shader> m_Shader;
    Ref<Engine::Shader> m_CubeShader;
    Ref<Engine::Shader> m_LightShader;
    Ref<Engine::Shader> m_DepthBufferShader;
    Ref<Engine::Shader> m_DebugShader;
    Ref<Engine::Shader> m_HitboxShader;

    Engine::Framebuffer* m_Framebuffer;
    Engine::Framebuffer* m_DebugFramebuffer;
    Engine::Framebuffer* m_DepthFramebuffer;
    Engine::VertexArray* m_DepthBufferVA;
    Engine::Skybox* m_Skybox{};

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
    bool m_DisableLighting{false};
    bool m_ShowDepthBuffer{false};
    bool m_LockCamera{};

    double velocity{};
    Hitbox* m_DebugCube;
};

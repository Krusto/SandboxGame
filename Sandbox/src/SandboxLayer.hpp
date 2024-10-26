#pragma once
#include <memory>
#include <Engine.hpp>

class SandboxLayer: public Engine::Layer
{
public:
    explicit SandboxLayer(const Engine::ApplicationSpec& spec);

    ~SandboxLayer() override {}

public:
    void Init(std::weak_ptr<Engine::Window> window) override;

    void OnAttach() override;

    void OnUpdate(float dt) override;

    void OnDetach() override;

    void Destroy() override;

    void OnImGuiDraw() override;

    void OnMouseMoveEvent(int width, int height) override{};

    void OnKeyboardEvent(int action, int key) override{};

    void OnWindowResizeEvent(int width, int height) override;

    void OnWindowShouldCloseEvent() override { SetShouldExit(true); };

    void Begin() override {}

    void End() override {}

protected:
    std::weak_ptr<Engine::Window> m_Window;

    ViewportSize m_ViewportSize;

    Engine::ApplicationSpec m_AppSpec;
};

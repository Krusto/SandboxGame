#pragma once
#include <memory>
#include <Engine.hpp>

class GUILayer: public Engine::Layer
{
public:
    GUILayer() = default;
    explicit GUILayer(const Engine::ApplicationSpec& spec);
    ~GUILayer() = default;

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
    double m_DeltaTime = 0.0f;

    ViewportSize m_ViewportSize;
    Engine::ApplicationSpec m_AppSpec;
    std::filesystem::path m_AssetsDirectory;
    std::filesystem::path m_ShadersDirectory;
    std::filesystem::path m_TexturesDirectory;
};

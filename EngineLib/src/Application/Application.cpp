//
// Created by Stoyanov, Krusto (K.S.) on 6/3/2022.
//

#include "Application.hpp"
#include <Renderer/RendererAPI.hpp>
#include <chrono>

Engine::Application* Engine::Application::s_Application;

inline static double GetTime()
{
    auto before_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(before_time.time_since_epoch())
            .count();
}

void Engine::Application::Run()
{

    LayerStack::InitLayers(m_Window);

    double dt = 0, before, after;
    while (!m_Window->ShouldClose())
    {
        before = GetTime();
        auto imgui = LayerStack::GetLayer("GUILayer");
        imgui->OnImGuiBegin();
        for (auto& layer: LayerStack::data())
        {
            if (layer->GetName() != "GUILayer")
            {
                layer->OnImGuiDraw();
                layer->OnUpdate(dt);
            }
        }
        imgui->OnImGuiEnd();
        m_Window->Update();
        after = GetTime();

        dt = after - before;
    }
}

void Engine::Application::Init(const Engine::ApplicationSpec& spec)
{
    m_ApplicationSpec = spec;
    m_ApplicationSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    Logger::Create();

    m_Window = Ref(new Window());
    m_Window->Create(WindowSpec{m_ApplicationSpec.ApplicationName, m_ApplicationSpec.width, m_ApplicationSpec.height});

    RendererSpec rendererSpec = {.SurfaceSize = {m_ApplicationSpec.width, m_ApplicationSpec.height}};
    Renderer::Init(rendererSpec, m_ApplicationSpec);
}

void Engine::Application::Destroy()
{

    LayerStack::PopLayers();
    LayerStack::DestroyLayers();

    Renderer::Shutdown();
    Renderer::Destroy();

    m_Window->Close();
    m_Window->Destroy();
    glfwTerminate();
}

Engine::Application::~Application() { Destroy(); }
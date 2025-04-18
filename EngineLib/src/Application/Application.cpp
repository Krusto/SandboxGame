//
// Created by Stoyanov, Krusto (K.S.) on 6/3/2022.
//

#include "Application.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/Loader.hpp>
#include <chrono>

Engine::Application* s_Application{};

inline static double GetTime()
{
    auto before_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(before_time.time_since_epoch())
            .count();
}

Engine::Application& Engine::Application::Get() { return *s_Application; }

void Engine::Application::Run()
{
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
    LoggerCreate();
    LoggerAttachTerminalHandler(LogPolicy::APPEND_POLICY);
    LOG_INFO("Application Init");

    m_ApplicationSpec = spec;
    m_ApplicationSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory);
    RendererSharedLoader::Load((m_ApplicationSpec.WorkingDirectory.string()).c_str());

    RendererSpec rendererSpec = {.SurfaceSize = {m_ApplicationSpec.width, m_ApplicationSpec.height}};
    Renderer::Init(rendererSpec, m_ApplicationSpec);
    LOG_DEBUG("%p\n", Renderer::GetInstance());
    m_Window = Engine::Allocator::Allocate<Window>();
    LOG_DEBUG("%p\n", Renderer::GetInstance());

    m_Window->Create(WindowSpec{m_ApplicationSpec.ApplicationName, m_ApplicationSpec.width, m_ApplicationSpec.height});
    LayerStack::InitLayers(m_Window, (m_ApplicationSpec.WorkingDirectory.string()).c_str());
}

void Engine::Application::Destroy()
{

    LayerStack::PopLayers();
    LayerStack::DestroyLayers();

    Renderer::Shutdown();
    Renderer::Destroy();

    m_Window->Close();
    m_Window->Destroy();
    Engine::Allocator::Deallocate(m_Window);
    LoggerDestroy();
}
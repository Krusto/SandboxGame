//
// Created by Stoyanov, Krusto (K.S.) on 6/3/2022.
//

#include "Application.hpp"
#include <Renderer/RendererAPI.hpp>

Engine::Application* Engine::Application::s_Application;

void Engine::Application::Run()
{

    LayerStack::InitLayers(m_Window);

    float dt = 0, before, after;
    while (!m_Window->ShouldClose())
    {
        for (auto& layer: LayerStack::data())
        {
            before = (float) glfwGetTime();
            layer->OnImGuiDraw();
            layer->OnUpdate(dt);

            after = (float) glfwGetTime();
            dt = after - before;
        }

        m_Window->Update();
    }
}

void Engine::Application::Init(const Engine::ApplicationSpec& spec)
{
    m_ApplicationSpec = spec;
    m_ApplicationSpec.WorkingDirectory = std::filesystem::absolute(spec.WorkingDirectory).string();
    Logger::Create();

    m_Window = Window::Create({m_ApplicationSpec.ApplicationName, 1280, 720});

    RendererSpec rendererSpec;
    rendererSpec.SurfaceSize = {1280, 720};

    Renderer::Init(rendererSpec, m_ApplicationSpec);
}

void Engine::Application::Destroy()
{
    m_Window->Close();

    LayerStack::DestroyLayers();
    LayerStack::PopLayers();

    Renderer::Shutdown();

    glfwDestroyWindow(m_Window->GetHandle());
    glfwTerminate();
}

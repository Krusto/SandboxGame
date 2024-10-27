//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once

#include <memory>
#include <functional>
#include <vector>

#include <glm/vec4.hpp>

#include "Window.hpp"
#include "Viewport.hpp"
#include "Shader.hpp"

#include <Renderer/GraphicsContext.hpp>

namespace Engine
{

    struct RendererSpec {
        ViewportSize SurfaceSize;
        std::vector<const char*> extensions;
    };

    class Entity;
    class RendererAPI;

    class Renderer
    {
    public:
        static void Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec);
        static void Destroy();
        static void InitImGUI(Ref<Window> window);
        static void Shutdown();
        static void BeginFrame();
        static void EndFrame();
        static void ClearColor(glm::vec4 color);
        static GraphicsContext* CreateGraphicsContext(GLFWwindow* handle);
        static RendererAPI* GetAPIInstance();

        static void Submit(const std::function<void(void)>& func);

        template <typename T>
        static void SubmitAndFlush(T func);

        static void Flush();

        inline static std::vector<std::function<void(void)>> s_FunctionContainer{};
        inline static RendererSpec s_RendererSpec{};
    };
}// namespace Engine

#include <Renderer/Renderer.impl>
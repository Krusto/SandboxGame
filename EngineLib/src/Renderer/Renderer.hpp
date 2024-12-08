//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once

#include <memory>
#include <functional>
#include <vector>

#include <glm/vec4.hpp>

#include "RendererCore.hpp"
#include "Viewport.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "TextureArray.hpp"
#include "CubemapTexture.hpp"
#include "Skybox.hpp"
#include "Framebuffer.hpp"
#include "VertexArray.hpp"
#include "DepthFunction.hpp"

#include <Renderer/GraphicsContext.hpp>
#include <Renderer/RendererCommand.hpp>

namespace Engine
{

    struct RendererSpec {
        ViewportSize SurfaceSize;
        std::vector<const char*> extensions;
    };

    class Entity;
    class RendererAPI;
    class Window;
    struct ApplicationSpec;

    class Renderer
    {
    public:
        static void Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec);
        static void Destroy();
        static void InitImGUI(Window* window);
        static void Shutdown();
        static void BeginFrame();
        static void EndFrame();
        static void SetViewport(ViewportSize size);
        static void BindDefaultFramebuffer();

        static GraphicsContext* CreateGraphicsContext(GLFWwindow* handle);
        static RendererAPI* GetAPIInstance();

        static void SwitchWireframeMode();
        static void SwitchFillMode();
        static void ChangeDepthFunction(DepthFunction depthFunction);

        static void RenderIndexed(const VertexArray* vertexArray, uint32_t indexCount = 0);
        static void SubmitRenderIndexed(VertexArray* vertexArray, uint32_t indexCount = 0);

        static void Submit(RendererCommand command);

        static void SubmitAndFlush(RendererCommand command);

        static void Flush();

        inline static std::vector<RendererCommand> s_CommandContainer{};
        inline static RendererSpec s_RendererSpec{};
    };
}// namespace Engine

#ifdef RENDERER_BUILD_DLL
class __declspec(dllexport) Engine::Renderer;
#else
class __declspec(dllimport) Engine::Renderer;
#endif

#include <Renderer/Renderer.impl>
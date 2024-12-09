//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once

#include <memory>
#include <functional>
#include <vector>

#include <glm/vec4.hpp>

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
#include <Renderer/Predefines.hpp>

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

    class EXPORT_RENDERER Renderer
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

#include <Renderer/Renderer.impl>
//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once

#include "Viewport.hpp"
#include "Renderer.hpp"
#include "DepthFunction.hpp"
#include "RendererCore.hpp"

namespace Engine
{

    struct ApplicationSpec;

    class RendererAPI
    {
    public:
        RendererAPI() = default;
        ~RendererAPI() = default;

    public:
        void Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec);
        void Shutdown();
        void BeginFrame();
        void EndFrame();
        void ClearColor(glm::vec4 color);
        void SwitchMode(uint32_t mode);
        void SetViewport(ViewportSize size);
        void BindDefaultFramebuffer() const;
        void RenderIndexed(const VertexArray* vertexArray, uint32_t indexCount) const;
        void ChangeDepthFunction(DepthFunction depthFunction);
    };
}// namespace Engine

#ifdef RENDERER_BUILD_DLL
class __declspec(dllexport) Engine::RendererAPI;
#else
class __declspec(dllimport) Engine::RendererAPI;
#endif
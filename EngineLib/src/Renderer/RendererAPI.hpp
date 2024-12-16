//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once
#include <glm/glm.hpp>
#include <Renderer/Predefines.hpp>
#include "Viewport.hpp"
#include "RendererSpec.hpp"
#include "DepthFunction.hpp"
#include "VertexArray.hpp"

namespace Engine
{

    struct ApplicationSpec;

    class EXPORT_RENDERER RendererAPI
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
        void RenderIndexed(VertexArray vertexArray, uint32_t indexCount) const;
        void ChangeDepthFunction(DepthFunction depthFunction);
    };
}// namespace Engine

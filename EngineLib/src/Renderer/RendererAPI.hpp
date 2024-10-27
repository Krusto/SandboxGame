//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once

#include "Window.hpp"
#include "Viewport.hpp"
#include "Renderer.hpp"

namespace Engine
{

    class RendererAPI
    {
    public:
        virtual ~RendererAPI() = default;

    public:
        virtual void Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec) = 0;
        virtual void Shutdown() = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void ClearColor(glm::vec4 color) = 0;
    };
}// namespace Engine
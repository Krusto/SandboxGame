#pragma once
#include <memory>

#include <Renderer/Renderer.hpp>
#include <Renderer/RendererAPI.hpp>

namespace Engine
{
    class OpenGLRenderer: public RendererAPI
    {
    public:
        OpenGLRenderer() = default;
        ~OpenGLRenderer() = default;

    public:
        virtual void Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec) override;
        virtual void Shutdown() override;

    public:
        virtual void BeginFrame() override;
        virtual void EndFrame() override;
        virtual void ClearColor(glm::vec4 color) override;
    };
}// namespace Engine
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
        virtual void SwitchMode(uint32_t mode) override;
        virtual void SetViewport(ViewportSize size) override;
        virtual void BindDefaultFramebuffer() const override;
        virtual void RenderIndexed(VertexArray* vertexArray, uint32_t indexCount) const override;
    };
}// namespace Engine
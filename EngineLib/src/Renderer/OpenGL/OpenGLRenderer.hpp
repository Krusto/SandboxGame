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

        virtual void Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec) override;
        virtual void Shutdown() override{};
        virtual void BeginFrame() override{};
        virtual void EndFrame() override{};
        virtual void RenderQuad() override{};
        virtual void RenderMesh() override{};
        virtual void RenderMeshWithMaterial() override{};

        virtual void ClearColor(glm::vec4 color) override;

    private:
    };
}// namespace Engine
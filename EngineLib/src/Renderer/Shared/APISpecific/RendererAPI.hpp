//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once
#include <glm/glm.hpp>
#include <Renderer/Predefines.hpp>
#include <Renderer/Shared/Viewport.hpp>
#include <Renderer/Shared/RendererSpec.hpp>
#include <Renderer/Shared/APISpecific/VertexArray.hpp>
#include <Renderer/Shared/DepthFunction.hpp>

typedef struct GLFWwindow GLFWwindow;
typedef struct ImDrawData ImDrawData;

namespace Engine
{

    struct ApplicationSpec;
    enum class RendererAPIType
    {
        OpenGL,
        Vulkan
    };
    struct RendererAPIData;

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
        void RenderIndexed(VertexArray vertexArray, uint32_t indexCount) const;
        void ChangeDepthFunction(DepthFunction depthFunction);
        void InitImGUI(GLFWwindow* window);
        void DestroyImGUI();
        void ImGuiNewFrame();
        void ImGuiRender(ImDrawData* drawData);
        RendererAPIType GetAPI() const;

    private:
        RendererAPIData* m_Data{};
    };
}// namespace Engine

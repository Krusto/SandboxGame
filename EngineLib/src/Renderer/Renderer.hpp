//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once

#include <memory>
#include <functional>
#include <vector>

#include <glm/vec4.hpp>

#include "DepthFunction.hpp"
#include "GraphicsContext.hpp"
#include "RendererCommand.hpp"
#include "ShaderDataType.hpp"
#include "ShaderUniform.hpp"
#include "VertexLayout.hpp"
#include "Viewport.hpp"
#include "RendererSpec.hpp"
#include "Predefines.hpp"
#include "Vertex.hpp"
#include "Camera.hpp"
#include "Shared/CubemapTexture.hpp"
#include "Shared/Framebuffer.hpp"
#include "Shared/Image.hpp"
#include "Shared/IndexBuffer.hpp"
#include "Shared/RendererAPI.hpp"
#include "Shared/Shader.hpp"
#include "Shared/Skybox.hpp"
#include "Shared/StorageBuffer.hpp"
#include "Shared/TextureArray.hpp"
#include "Shared/VertexBuffer.hpp"
#include "Shared/VertexArray.hpp"

typedef struct GLFWwindow GLFWwindow;
typedef struct ImDrawData ImDrawData;

namespace Engine
{

    class Entity;
    class RendererAPI;
    struct ApplicationSpec;

    class EXPORT_RENDERER Renderer
    {
    public:
        static void Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec);
        static void Destroy();
        static void InitImGUI(GLFWwindow* window);
        static void ImGuiNewFrame();
        static void ImGuiRender(ImDrawData* drawData);
        static void DestroyImGUI();
        static void Shutdown();
        static void BeginFrame();
        static void EndFrame();
        static void SetViewport(ViewportSize size);
        static void BindDefaultFramebuffer();

        static RendererAPI* GetAPIInstance();

        static void SwitchWireframeMode();
        static void SwitchFillMode();
        static void ChangeDepthFunction(DepthFunction depthFunction);

        static void RenderIndexed(VertexArray vertexArray, uint32_t indexCount = 0);
        static void SubmitRenderIndexed(VertexArray vertexArray, uint32_t indexCount = 0);

        static void Submit(RendererCommand command);

        static void SubmitAndFlush(RendererCommand command);

        static void Flush();

        inline static std::vector<RendererCommand> s_CommandContainer{};
        inline static RendererSpec s_RendererSpec{};
    };
}// namespace Engine

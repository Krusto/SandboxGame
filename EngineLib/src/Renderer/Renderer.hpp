//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once

#include <memory>
#include <functional>
#include <vector>

#include <glm/vec4.hpp>

#include "Shared/DepthFunction.hpp"
#include "Shared/GraphicsContext.hpp"
#include "Shared/RendererCommand.hpp"
#include "Shared/ShaderDataType.hpp"
#include "Shared/ShaderUniform.hpp"
#include "Shared/VertexLayout.hpp"
#include "Shared/Viewport.hpp"
#include "Shared/RendererSpec.hpp"
#include "Shared/Vertex.hpp"
#include "Shared/Camera.hpp"
#include "Shared/APISpecific/CubemapTexture.hpp"
#include "Shared/APISpecific/Framebuffer.hpp"
#include "Shared/APISpecific/Image.hpp"
#include "Shared/APISpecific/IndexBuffer.hpp"
#include "Shared/APISpecific/RendererAPI.hpp"
#include "Shared/APISpecific/Shader.hpp"
#include "Shared/APISpecific/Skybox.hpp"
#include "Shared/APISpecific/StorageBuffer.hpp"
#include "Shared/APISpecific/TextureArray.hpp"
#include "Shared/APISpecific/VertexBuffer.hpp"
#include "Shared/APISpecific/VertexArray.hpp"

typedef struct GLFWwindow GLFWwindow;
typedef struct ImDrawData ImDrawData;

namespace Engine
{

    class Entity;
    class RendererAPI;
    struct ApplicationSpec;

    class Renderer
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

    public:
#include <Renderer/Shared/function_pointers.h>
    };
}// namespace Engine

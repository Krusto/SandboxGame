//
// Created by Stoyanov, Krusto (K.S.) on 6/7/2022.
//
#pragma once

#include <memory>
#include <functional>
#include <vector>

#include <glm/vec4.hpp>

#include "Camera.hpp"
#include "CubemapTexture.hpp"
#include "DepthFunction.hpp"
#include "Framebuffer.hpp"
#include "GraphicsContext.hpp"
#include "Image.hpp"
#include "IndexBuffer.hpp"
#include "RendererAPI.hpp"
#include "RendererCommand.hpp"
#include "Shader.hpp"
#include "ShaderDataType.hpp"
#include "ShaderUniform.hpp"
#include "Skybox.hpp"
#include "StorageBuffer.hpp"
#include "TextureArray.hpp"
#include "Vertex.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "VertexLayout.hpp"
#include "Viewport.hpp"
#include "RendererSpec.hpp"
#include "Predefines.hpp"

namespace Engine
{

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

        static void RenderIndexed(VertexArray vertexArray, uint32_t indexCount = 0);
        static void SubmitRenderIndexed(VertexArray vertexArray, uint32_t indexCount = 0);

        static void Submit(RendererCommand command);

        static void SubmitAndFlush(RendererCommand command);

        static void Flush();

        inline static std::vector<RendererCommand> s_CommandContainer{};
        inline static RendererSpec s_RendererSpec{};
    };
}// namespace Engine

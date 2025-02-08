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

    extern "C" {
#ifndef Renderer_EXPORTS
    inline __declspec(dllexport) void* s_Renderer;
#else
    __declspec(dllimport) void* s_Renderer;
#endif
    }

    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

    public:
        inline static Renderer* GetInstance() { return (Renderer*) s_Renderer; }

        static Renderer* Create();

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

        static const RendererAPI* GetAPIInstance();

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
        // Graphics Context
        typedef void (*RendererContextCreate_T)(void* handle, void* window);
        RendererContextCreate_T RendererContextCreate;

        typedef void (*RendererContextInit_T)(void* handle);
        RendererContextInit_T RendererContextInit;

        typedef void (*RendererContextDestroy_T)(void* handle);
        RendererContextDestroy_T RendererContextDestroy;

        typedef void (*RendererContextSwapBuffers_T)(void* window);
        RendererContextSwapBuffers_T RendererContextSwapBuffers;

        typedef void (*RendererContextAddDebugMessanger_T)(void* window);
        RendererContextAddDebugMessanger_T RendererContextAddDebugMessanger;

        typedef void (*RendererContextSetupWindowHints_T)(void* window);
        RendererContextSetupWindowHints_T RendererContextSetupWindowHints;

        typedef void (*_MessageCallback_T)(unsigned int source, unsigned int type, unsigned int id,
                                           unsigned int severity, int length, char const* message,
                                           void const* user_param);
        _MessageCallback_T _MessageCallback;

        //Cubemap texture
        typedef void (*CubemapTextureLoad_T)(void** data, void* cubemapName, void* paths);
        CubemapTextureLoad_T CubemapTextureLoad;

        typedef void (*CubemapTextureDestroy_T)(void** data);
        CubemapTextureDestroy_T CubemapTextureDestroy;

        typedef void* (*CubemapTextureGetSpec_T)(void* handle);
        CubemapTextureGetSpec_T CubemapTextureGetSpec;

        typedef int (*CubemapTextureGetID_T)(void* handle);
        CubemapTextureGetID_T CubemapTextureGetID;

        typedef void (*CubemapTextureBind_T)(void* data, unsigned int index);
        CubemapTextureBind_T CubemapTextureBind;

        //Image
        typedef void (*ImageInit_T)(void** data, uint8_t* imageData, size_t width, size_t height, uint8_t type);
        ImageInit_T ImageInit;

        typedef void (*ImageDestroy_T)(void** data);
        ImageDestroy_T ImageDestroy;

        typedef uint32_t (*ImageGetID_T)(void* data);
        ImageGetID_T ImageGetID;

        typedef void (*ImageBind_T)(void* data, uint32_t location);
        ImageBind_T ImageBind;

        //Framebuffer
        typedef void (*FramebufferInit_T)(void** data, unsigned int width, unsigned int height,
                                          unsigned int isDepthMap);
        FramebufferInit_T FramebufferInit;

        typedef void (*FramebufferDestroy_T)(void** data);
        FramebufferDestroy_T FramebufferDestroy;

        typedef void (*FramebufferBindColorTexture_T)(void* data, unsigned int slot);
        FramebufferBindColorTexture_T FramebufferBindColorTexture;

        typedef void (*FramebufferBindDepthTexture_T)(void* data, unsigned int slot);
        FramebufferBindDepthTexture_T FramebufferBindDepthTexture;

        typedef void (*FramebufferClearColor_T)(void* data, void* color);
        FramebufferClearColor_T FramebufferClearColor;

        typedef void (*FramebufferClearDepth_T)(void* data);
        FramebufferClearDepth_T FramebufferClearDepth;

        typedef void (*FramebufferBind_T)(void* data);
        FramebufferBind_T FramebufferBind;

        typedef void (*FramebufferUnbind_T)(void* data);
        FramebufferUnbind_T FramebufferUnbind;

        typedef int (*FramebufferGetID_T)(void* data);
        FramebufferGetID_T FramebufferGetID;

        typedef int (*FramebufferGetColorTextureID_T)(void* data);
        FramebufferGetColorTextureID_T FramebufferGetColorTextureID;

        typedef int (*FramebufferGetDepthTextureID_T)(void* data);
        FramebufferGetDepthTextureID_T FramebufferGetDepthTextureID;

        typedef unsigned int (*FramebufferGetWidth_T)(void* data);
        FramebufferGetWidth_T FramebufferGetWidth;

        typedef unsigned int (*FramebufferGetHeight_T)(void* data);
        FramebufferGetHeight_T FramebufferGetHeight;

        //Vertex Array
        typedef void (*VertexArrayInit_T)(void** data, unsigned int indexCount);
        VertexArrayInit_T VertexArrayInit;

        typedef void (*VertexArrayDestroy_T)(void** data);
        VertexArrayDestroy_T VertexArrayDestroy;

        typedef void (*VertexArrayBind_T)(void* data);
        VertexArrayBind_T VertexArrayBind;

        typedef void (*VertexArrayUnbind_T)(void* data);
        VertexArrayUnbind_T VertexArrayUnbind;

        typedef void (*VertexArrayAddVertexBuffer_T)(void* data, void* layout, float* vertexData, unsigned int length);
        VertexArrayAddVertexBuffer_T VertexArrayAddVertexBuffer;

        typedef void (*VertexArrayAddIndexBuffer_T)(void* data, unsigned int* indexData, unsigned int length);
        VertexArrayAddIndexBuffer_T VertexArrayAddIndexBuffer;

        typedef size_t (*VertexArrayGetIndexCount_T)(void* data);
        VertexArrayGetIndexCount_T VertexArrayGetIndexCount;

        typedef unsigned int (*VertexArrayGetID_T)(void* data);
        VertexArrayGetID_T VertexArrayGetID;

        //Vertex Buffer
        typedef void (*VertexBufferInit_T)(void** data, void* vertexArray, void* vertexLayout, float* vertices,
                                           unsigned int length);
        VertexBufferInit_T VertexBufferInit;

        typedef void (*VertexBufferDestroy_T)(void** data);
        VertexBufferDestroy_T VertexBufferDestroy;

        typedef void (*VertexBufferBind_T)(void* data);
        VertexBufferBind_T VertexBufferBind;

        typedef size_t (*VertexBufferGetSize_T)(void* data);
        VertexBufferGetSize_T VertexBufferGetSize;

        typedef unsigned int (*VertexBufferGetID_T)(void* data);
        VertexBufferGetID_T VertexBufferGetID;

        //Index Buffer
        typedef void (*IndexBufferInit_T)(void** data, void* vertexArray, unsigned int* indexData, unsigned int length);
        IndexBufferInit_T IndexBufferInit;

        typedef void (*IndexBufferDestroy_T)(void** data);
        IndexBufferDestroy_T IndexBufferDestroy;

        typedef void (*IndexBufferBind_T)(void* data);
        IndexBufferBind_T IndexBufferBind;

        typedef size_t (*IndexBufferGetSize_T)(void* data);
        IndexBufferGetSize_T IndexBufferGetSize;

        typedef size_t (*IndexBufferGetLength_T)(void* data);
        IndexBufferGetLength_T IndexBufferGetLength;

        typedef unsigned int (*IndexBufferGetID_T)(void* data);
        IndexBufferGetID_T IndexBufferGetID;

        //Texture Array
        typedef void (*TextureArrayLoad_T)(void** data, const char* textureName, char** paths, unsigned int count);
        TextureArrayLoad_T TextureArrayLoad;

        typedef void (*TextureArrayDestroy_T)(void** data);
        TextureArrayDestroy_T TextureArrayDestroy;

        typedef void (*TextureArrayBind_T)(void* data, unsigned int index);
        TextureArrayBind_T TextureArrayBind;

        typedef const char* (*TextureArrayGetName_T)(void* data);
        TextureArrayGetName_T TextureArrayGetName;

        typedef unsigned int (*TextureArrayGetID_T)(void* data);
        TextureArrayGetID_T TextureArrayGetID;

        typedef unsigned int (*TextureArrayGetCount_T)(void* data);
        TextureArrayGetCount_T TextureArrayGetCount;

        typedef unsigned int (*TextureArrayGetWidth_T)(void* data);
        TextureArrayGetWidth_T TextureArrayGetWidth;

        typedef unsigned int (*TextureArrayGetHeight_T)(void* data);
        TextureArrayGetHeight_T TextureArrayGetHeight;

        typedef unsigned int (*TextureArrayGetChannels_T)(void* data);
        TextureArrayGetChannels_T TextureArrayGetChannels;

        //Storage Buffer
        typedef void (*StorageBufferInit_T)(void** data, void* vertexArray, char* storageData, unsigned int length,
                                            int type);
        StorageBufferInit_T StorageBufferInit;

        typedef void (*StorageBufferDestroy_T)(void** data);
        StorageBufferDestroy_T StorageBufferDestroy;

        typedef void (*StorageBufferBind_T)(void* data, unsigned int location);
        StorageBufferBind_T StorageBufferBind;

        typedef unsigned int (*StorageBufferGetID_T)(void* data);
        StorageBufferGetID_T StorageBufferGetID;

        typedef void (*StorageBufferUnbind_T)(void* data);
        StorageBufferUnbind_T StorageBufferUnbind;

        //GUIContext
        typedef void (*GUIContextInit_T)(void** data, void* window);
        GUIContextInit_T GUIContextInit;

        typedef void (*GUIContextDestroy_T)(void** data);
        GUIContextDestroy_T GUIContextDestroy;

        typedef void (*GUIContextBegin_T)(void* data);
        GUIContextBegin_T GUIContextBegin;

        typedef void (*GUIContextEnd_T)(void* data, void* drawData);
        GUIContextEnd_T GUIContextEnd;

        //RebdererAPI
        typedef void (*RendererAPIInit_T)(void** data, void* rendererSpec, void* applicationSpec);
        RendererAPIInit_T RendererAPIInit;

        typedef void (*RendererAPIDestroy_T)(void** data);
        RendererAPIDestroy_T RendererAPIDestroy;

        typedef void (*RendererAPISetClearColor_T)(void* data, float r, float g, float b, float a);
        RendererAPISetClearColor_T RendererAPISetClearColor;

        typedef void (*RendererAPISwitchPolygonMode_T)(void* data, unsigned int mode);
        RendererAPISwitchPolygonMode_T RendererAPISwitchPolygonMode;

        typedef void (*RendererAPISetViewport_T)(void* data, float width, float height);
        RendererAPISetViewport_T RendererAPISetViewport;

        typedef void (*RendererAPIBindDefaultFramebuffer_T)(void* data);
        RendererAPIBindDefaultFramebuffer_T RendererAPIBindDefaultFramebuffer;

        typedef void (*RendererAPIRenderIndexed_T)(void* data, unsigned int count);
        RendererAPIRenderIndexed_T RendererAPIRenderIndexed;

        typedef void (*RendererAPIChangeDepthFunc_T)(void* data, unsigned int func);
        RendererAPIChangeDepthFunc_T RendererAPIChangeDepthFunc;

        // typedef void (*RendererAPIChangeBlendFunc_T)(void* data, unsigned int src, unsigned int dst);
        //   RendererAPIChangeBlendFunc_T RendererAPIChangeBlendFunc;

        typedef void (*RendererAPIInitIMGUI_T)(void* renderer_instance, void* data, void* window);
        RendererAPIInitIMGUI_T RendererAPIInitIMGUI;

        typedef void (*RendererAPIDestroyIMGUI_T)(void* data);
        RendererAPIDestroyIMGUI_T RendererAPIDestroyIMGUI;

        typedef void (*RendererAPIIMGUIBegin_T)(void* data);
        RendererAPIIMGUIBegin_T RendererAPIIMGUIBegin;

        typedef void (*RendererAPIIMGUIEnd_T)(void* data, void* drawData);
        RendererAPIIMGUIEnd_T RendererAPIIMGUIEnd;

        //Shader
        typedef void (*ShaderLoad_T)(void** data, const char* path);
        ShaderLoad_T ShaderLoad;

        typedef void (*ShaderDestroy_T)(void** data);
        ShaderDestroy_T ShaderDestroy;

        typedef void (*ShaderBind_T)(void* data);
        ShaderBind_T ShaderBind;

        typedef void (*ShaderReload_T)(void* data, int recompile);
        ShaderReload_T ShaderReload;

        typedef unsigned int (*ShaderGetID_T)(void* data);
        ShaderGetID_T ShaderGetID;

        typedef char* (*ShaderGetName_T)(void* data);
        ShaderGetName_T ShaderGetName;

        typedef char* (*ShaderGetPath_T)(void* data);
        ShaderGetPath_T ShaderGetPath;

        typedef void (*ShaderSetUniformI_T)(void* data, const char* name, int value);
        ShaderSetUniformI_T ShaderSetUniformI;

        typedef void (*ShaderSetUniformUI_T)(void* data, const char* name, unsigned int value);
        ShaderSetUniformUI_T ShaderSetUniformUI;

        typedef void (*ShaderSetUniformF_T)(void* data, const char* name, float value);
        ShaderSetUniformF_T ShaderSetUniformF;

        typedef void (*ShaderSetUniform2F_T)(void* data, const char* name, float x, float y);
        ShaderSetUniform2F_T ShaderSetUniform2F;

        typedef void (*ShaderSetUniform3F_T)(void* data, const char* name, float x, float y, float z);
        ShaderSetUniform3F_T ShaderSetUniform3F;

        typedef void (*ShaderSetUniform4F_T)(void* data, const char* name, float x, float y, float z, float w);
        ShaderSetUniform4F_T ShaderSetUniform4F;

        typedef void (*ShaderSetUniformMat4_T)(void* data, const char* name, void* matrix);
        ShaderSetUniformMat4_T ShaderSetUniformMat4;

        typedef void (*ShaderSetUniformMat3_T)(void* data, const char* name, void* matrix);
        ShaderSetUniformMat3_T ShaderSetUniformMat3;

        typedef void (*ShaderSetUniform2I_T)(void* data, const char* name, int x, int y);
        ShaderSetUniform2I_T ShaderSetUniform2I;

        typedef void (*ShaderSetUniform3I_T)(void* data, const char* name, int x, int y, int z);
        ShaderSetUniform3I_T ShaderSetUniform3I;

        typedef void (*ShaderSetUniform4I_T)(void* data, const char* name, int x, int y, int z, int w);
        ShaderSetUniform4I_T ShaderSetUniform4I;
    };


}// namespace Engine

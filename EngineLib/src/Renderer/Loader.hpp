#pragma once
#include <LibraryLoader.h>
#include <Renderer/Renderer.hpp>
#include <string>
#define _LibStr(n) #n
#define LibName _LibStr(RendererOpenGL_)
#define _LoadSymbol2(funcName, funcNameStr)                                                                            \
    Renderer::GetInstance()->funcName = s_Loader.GetSymbol<decltype(Renderer::funcName)>(funcNameStr)
#define _LoadSymbol(funcName) _LoadSymbol2(funcName, #funcName)

namespace Engine
{
    class RendererSharedLoader
    {
    public:
        inline static void Load(std::string_view working_directory)
        {
            std::string path = std::string(working_directory.data()) + "/build/bin/Debug/RendererOpenGL.dll";
            if (!s_Loader.Init(path)) { return; }

            Renderer::Create();
            //Graphics Context
            _LoadSymbol(RendererContextCreate);
            _LoadSymbol(RendererContextDestroy);
            _LoadSymbol(RendererContextInit);
            _LoadSymbol(RendererContextSwapBuffers);
            _LoadSymbol(RendererContextAddDebugMessanger);
            _LoadSymbol(RendererContextSetupWindowHints);
            _LoadSymbol(_MessageCallback);

            //Image
            _LoadSymbol(ImageInit);
            _LoadSymbol(ImageDestroy);
            _LoadSymbol(ImageGetID);
            _LoadSymbol(ImageBind);

            //Cubemap texture
            _LoadSymbol(CubemapTextureLoad);
            _LoadSymbol(CubemapTextureDestroy);
            _LoadSymbol(CubemapTextureGetSpec);
            _LoadSymbol(CubemapTextureGetID);
            _LoadSymbol(CubemapTextureBind);

            //Framebuffer
            _LoadSymbol(FramebufferInit);
            _LoadSymbol(FramebufferDestroy);
            _LoadSymbol(FramebufferBind);
            _LoadSymbol(FramebufferUnbind);
            _LoadSymbol(FramebufferGetID);
            _LoadSymbol(FramebufferGetWidth);
            _LoadSymbol(FramebufferGetHeight);
            _LoadSymbol(FramebufferBindColorTexture);
            _LoadSymbol(FramebufferBindDepthTexture);
            _LoadSymbol(FramebufferGetColorTextureID);
            _LoadSymbol(FramebufferGetDepthTextureID);
            _LoadSymbol(FramebufferClearColor);
            _LoadSymbol(FramebufferClearDepth);

            //Vertex Array
            _LoadSymbol(VertexArrayInit);
            _LoadSymbol(VertexArrayDestroy);
            _LoadSymbol(VertexArrayBind);
            _LoadSymbol(VertexArrayUnbind);
            _LoadSymbol(VertexArrayGetID);
            _LoadSymbol(VertexArrayAddVertexBuffer);
            _LoadSymbol(VertexArrayAddIndexBuffer);
            _LoadSymbol(VertexArrayGetIndexCount);
            _LoadSymbol(VertexArrayGetID);

            //Vertex Buffer
            _LoadSymbol(VertexBufferInit);
            _LoadSymbol(VertexBufferDestroy);
            _LoadSymbol(VertexBufferBind);
            _LoadSymbol(VertexBufferGetSize);
            _LoadSymbol(VertexBufferGetID);

            //Index Buffer
            _LoadSymbol(IndexBufferInit);
            _LoadSymbol(IndexBufferDestroy);
            _LoadSymbol(IndexBufferBind);
            _LoadSymbol(IndexBufferGetSize);
            _LoadSymbol(IndexBufferGetLength);
            _LoadSymbol(IndexBufferGetID);

            //Texture Array
            _LoadSymbol(TextureArrayLoad);
            _LoadSymbol(TextureArrayDestroy);
            _LoadSymbol(TextureArrayBind);
            _LoadSymbol(TextureArrayGetName);
            _LoadSymbol(TextureArrayGetID);
            _LoadSymbol(TextureArrayGetCount);
            _LoadSymbol(TextureArrayGetWidth);
            _LoadSymbol(TextureArrayGetHeight);
            _LoadSymbol(TextureArrayGetChannels);

            //Storage Buffer
            _LoadSymbol(StorageBufferInit);
            _LoadSymbol(StorageBufferDestroy);
            _LoadSymbol(StorageBufferBind);
            _LoadSymbol(StorageBufferGetID);
            _LoadSymbol(StorageBufferUnbind);

            //GUIContext
            _LoadSymbol(GUIContextInit);
            _LoadSymbol(GUIContextDestroy);
            _LoadSymbol(GUIContextBegin);
            _LoadSymbol(GUIContextEnd);

            //Renderer
            _LoadSymbol(RendererAPIInit);
            _LoadSymbol(RendererAPIDestroy);
            _LoadSymbol(RendererAPISetClearColor);
            _LoadSymbol(RendererAPISwitchPolygonMode);
            _LoadSymbol(RendererAPISetViewport);
            _LoadSymbol(RendererAPIBindDefaultFramebuffer);
            _LoadSymbol(RendererAPIRenderIndexed);
            _LoadSymbol(RendererAPIChangeDepthFunc);
            _LoadSymbol(RendererAPIInitIMGUI);
            _LoadSymbol(RendererAPIDestroyIMGUI);
            _LoadSymbol(RendererAPIIMGUIBegin);
            _LoadSymbol(RendererAPIIMGUIEnd);

            //Shader
            _LoadSymbol(ShaderLoad);
            _LoadSymbol(ShaderDestroy);
            _LoadSymbol(ShaderBind);
            _LoadSymbol(ShaderReload);
            _LoadSymbol(ShaderGetID);
            _LoadSymbol(ShaderGetName);
            _LoadSymbol(ShaderGetPath);
            _LoadSymbol(ShaderSetUniformI);
            _LoadSymbol(ShaderSetUniformUI);
            _LoadSymbol(ShaderSetUniformF);
            _LoadSymbol(ShaderSetUniform2F);
            _LoadSymbol(ShaderSetUniform3F);
            _LoadSymbol(ShaderSetUniform4F);
            _LoadSymbol(ShaderSetUniformMat3);
            _LoadSymbol(ShaderSetUniformMat4);
            _LoadSymbol(ShaderSetUniform2I);
            _LoadSymbol(ShaderSetUniform3I);
            _LoadSymbol(ShaderSetUniform4I);
        }

        inline static void Unload() { s_Loader.Unload(); }

    public:
        inline static LibraryLoader s_Loader{};
    };
}// namespace Engine
#pragma once
#include <cstdint>


// Graphics Context
typedef void (*RendererContextCreate_T)(void* handle, void* window);
static RendererContextCreate_T RendererContextCreate;

typedef void (*RendererContextInit_T)(void* handle);
inline static RendererContextInit_T RendererContextInit;

typedef void (*RendererContextDestroy_T)(void* handle);
inline static RendererContextDestroy_T RendererContextDestroy;

typedef void (*RendererContextSwapBuffers_T)(void* window);
inline static RendererContextSwapBuffers_T RendererContextSwapBuffers;

typedef void (*RendererContextAddDebugMessanger_T)(void* window);
inline static RendererContextAddDebugMessanger_T RendererContextAddDebugMessanger;

typedef void (*RendererContextSetupWindowHints_T)(void* window);
inline static RendererContextSetupWindowHints_T RendererContextSetupWindowHints;

typedef void (*_MessageCallback_T)(unsigned int source, unsigned int type, unsigned int id, unsigned int severity,
                                   int length, char const* message, void const* user_param);
inline static _MessageCallback_T _MessageCallback;

//Cubemap texture
typedef void (*CubemapTextureLoad_T)(void** data, void* cubemapName, void* paths);
inline static CubemapTextureLoad_T CubemapTextureLoad;

typedef void (*CubemapTextureDestroy_T)(void** data);
inline static CubemapTextureDestroy_T CubemapTextureDestroy;

typedef void* (*CubemapTextureGetSpec_T)(void* handle);
inline static CubemapTextureGetSpec_T CubemapTextureGetSpec;

typedef int (*CubemapTextureGetID_T)(void* handle);
inline static CubemapTextureGetID_T CubemapTextureGetID;

typedef void (*CubemapTextureBind_T)(void* data, unsigned int index);
inline static CubemapTextureBind_T CubemapTextureBind;

//Image
typedef void (*ImageInit_T)(void** data, uint8_t* imageData, size_t width, size_t height, uint8_t type);
inline static ImageInit_T ImageInit;

typedef void (*ImageDestroy_T)(void** data);
inline static ImageDestroy_T ImageDestroy;

typedef uint32_t (*ImageGetID_T)(void* data);
inline static ImageGetID_T ImageGetID;

typedef void (*ImageBind_T)(void* data, uint32_t location);
inline static ImageBind_T ImageBind;

//Framebuffer
typedef void (*FramebufferInit_T)(void** data, unsigned int width, unsigned int height, unsigned int isDepthMap);
inline static FramebufferInit_T FramebufferInit;

typedef void (*FramebufferDestroy_T)(void** data);
inline static FramebufferDestroy_T FramebufferDestroy;

typedef void (*FramebufferBindColorTexture_T)(void* data, unsigned int slot);
inline static FramebufferBindColorTexture_T FramebufferBindColorTexture;

typedef void (*FramebufferBindDepthTexture_T)(void* data, unsigned int slot);
inline static FramebufferBindDepthTexture_T FramebufferBindDepthTexture;

typedef void (*FramebufferClearColor_T)(void* data, void* color);
inline static FramebufferClearColor_T FramebufferClearColor;

typedef void (*FramebufferClearDepth_T)(void* data);
inline static FramebufferClearDepth_T FramebufferClearDepth;

typedef void (*FramebufferBind_T)(void* data);
inline static FramebufferBind_T FramebufferBind;

typedef void (*FramebufferUnbind_T)(void* data);
inline static FramebufferUnbind_T FramebufferUnbind;

typedef int (*FramebufferGetID_T)(void* data);
inline static FramebufferGetID_T FramebufferGetID;

typedef int (*FramebufferGetColorTextureID_T)(void* data);
inline static FramebufferGetColorTextureID_T FramebufferGetColorTextureID;

typedef int (*FramebufferGetDepthTextureID_T)(void* data);
inline static FramebufferGetDepthTextureID_T FramebufferGetDepthTextureID;

typedef unsigned int (*FramebufferGetWidth_T)(void* data);
inline static FramebufferGetWidth_T FramebufferGetWidth;

typedef unsigned int (*FramebufferGetHeight_T)(void* data);
inline static FramebufferGetHeight_T FramebufferGetHeight;

//Vertex Array
typedef void (*VertexArrayInit_T)(void** data, unsigned int indexCount);
inline static VertexArrayInit_T VertexArrayInit;

typedef void (*VertexArrayDestroy_T)(void** data);
inline static VertexArrayDestroy_T VertexArrayDestroy;

typedef void (*VertexArrayBind_T)(void* data);
inline static VertexArrayBind_T VertexArrayBind;

typedef void (*VertexArrayUnbind_T)(void* data);
inline static VertexArrayUnbind_T VertexArrayUnbind;

typedef void (*VertexArrayAddVertexBuffer_T)(void* data, void* layout, float* vertexData, unsigned int length);
inline static VertexArrayAddVertexBuffer_T VertexArrayAddVertexBuffer;

typedef void (*VertexArrayAddIndexBuffer_T)(void* data, unsigned int* indexData, unsigned int length);
inline static VertexArrayAddIndexBuffer_T VertexArrayAddIndexBuffer;

typedef size_t (*VertexArrayGetIndexCount_T)(void* data);
inline static VertexArrayGetIndexCount_T VertexArrayGetIndexCount;

typedef unsigned int (*VertexArrayGetID_T)(void* data);
inline static VertexArrayGetID_T VertexArrayGetID;

//Vertex Buffer
typedef void (*VertexBufferInit_T)(void** data, void* vertexArray, void* vertexLayout, float* vertices,
                                   unsigned int length);
inline static VertexBufferInit_T VertexBufferInit;

typedef void (*VertexBufferDestroy_T)(void** data);
inline static VertexBufferDestroy_T VertexBufferDestroy;

typedef void (*VertexBufferBind_T)(void* data);
inline static VertexBufferBind_T VertexBufferBind;

typedef size_t (*VertexBufferGetSize_T)(void* data);
inline static VertexBufferGetSize_T VertexBufferGetSize;

typedef unsigned int (*VertexBufferGetID_T)(void* data);
inline static VertexBufferGetID_T VertexBufferGetID;

//Index Buffer
typedef void (*IndexBufferInit_T)(void** data, void* vertexArray, unsigned int* indexData, unsigned int length);
inline static IndexBufferInit_T IndexBufferInit;

typedef void (*IndexBufferDestroy_T)(void** data);
inline static IndexBufferDestroy_T IndexBufferDestroy;

typedef void (*IndexBufferBind_T)(void* data);
inline static IndexBufferBind_T IndexBufferBind;

typedef size_t (*IndexBufferGetSize_T)(void* data);
inline static IndexBufferGetSize_T IndexBufferGetSize;

typedef size_t (*IndexBufferGetLength_T)(void* data);
inline static IndexBufferGetLength_T IndexBufferGetLength;

typedef unsigned int (*IndexBufferGetID_T)(void* data);
inline static IndexBufferGetID_T IndexBufferGetID;

//Texture Array
typedef void (*TextureArrayLoad_T)(void** data, const char* textureName, char** paths, unsigned int count);
inline static TextureArrayLoad_T TextureArrayLoad;

typedef void (*TextureArrayDestroy_T)(void** data);
inline static TextureArrayDestroy_T TextureArrayDestroy;

typedef void (*TextureArrayBind_T)(void* data, unsigned int index);
inline static TextureArrayBind_T TextureArrayBind;

typedef const char* (*TextureArrayGetName_T)(void* data);
inline static TextureArrayGetName_T TextureArrayGetName;

typedef unsigned int (*TextureArrayGetID_T)(void* data);
inline static TextureArrayGetID_T TextureArrayGetID;

typedef unsigned int (*TextureArrayGetCount_T)(void* data);
inline static TextureArrayGetCount_T TextureArrayGetCount;

typedef unsigned int (*TextureArrayGetWidth_T)(void* data);
inline static TextureArrayGetWidth_T TextureArrayGetWidth;

typedef unsigned int (*TextureArrayGetHeight_T)(void* data);
inline static TextureArrayGetHeight_T TextureArrayGetHeight;

typedef unsigned int (*TextureArrayGetChannels_T)(void* data);
inline static TextureArrayGetChannels_T TextureArrayGetChannels;

//Storage Buffer
typedef void (*StorageBufferInit_T)(void** data, void* vertexArray, char* storageData, unsigned int length, int type);
inline static StorageBufferInit_T StorageBufferInit;

typedef void (*StorageBufferDestroy_T)(void** data);
inline static StorageBufferDestroy_T StorageBufferDestroy;

typedef void (*StorageBufferBind_T)(void* data, unsigned int location);
inline static StorageBufferBind_T StorageBufferBind;

typedef unsigned int (*StorageBufferGetID_T)(void* data);
inline static StorageBufferGetID_T StorageBufferGetID;

typedef void (*StorageBufferUnbind_T)(void* data);
inline static StorageBufferUnbind_T StorageBufferUnbind;

//GUIContext
typedef void (*GUIContextInit_T)(void** data, void* window);
inline static GUIContextInit_T GUIContextInit;

typedef void (*GUIContextDestroy_T)(void** data);
inline static GUIContextDestroy_T GUIContextDestroy;

typedef void (*GUIContextBegin_T)(void* data);
inline static GUIContextBegin_T GUIContextBegin;

typedef void (*GUIContextEnd_T)(void* data, void* drawData);
inline static GUIContextEnd_T GUIContextEnd;

//RebdererAPI
typedef void (*RendererAPIInit_T)(void** data, void* rendererSpec, void* applicationSpec);
inline static RendererAPIInit_T RendererAPIInit;

typedef void (*RendererAPIDestroy_T)(void** data);
inline static RendererAPIDestroy_T RendererAPIDestroy;

typedef void (*RendererAPISetClearColor_T)(void* data, float r, float g, float b, float a);
inline static RendererAPISetClearColor_T RendererAPISetClearColor;

typedef void (*RendererAPISwitchPolygonMode_T)(void* data, unsigned int mode);
inline static RendererAPISwitchPolygonMode_T RendererAPISwitchPolygonMode;

typedef void (*RendererAPISetViewport_T)(void* data, float width, float height);
inline static RendererAPISetViewport_T RendererAPISetViewport;

typedef void (*RendererAPIBindDefaultFramebuffer_T)(void* data);
inline static RendererAPIBindDefaultFramebuffer_T RendererAPIBindDefaultFramebuffer;

typedef void (*RendererAPIRenderIndexed_T)(void* data, unsigned int count);
inline static RendererAPIRenderIndexed_T RendererAPIRenderIndexed;

typedef void (*RendererAPIChangeDepthFunc_T)(void* data, unsigned int func);
inline static RendererAPIChangeDepthFunc_T RendererAPIChangeDepthFunc;

// typedef void (*RendererAPIChangeBlendFunc_T)(void* data, unsigned int src, unsigned int dst);
// inline static  RendererAPIChangeBlendFunc_T RendererAPIChangeBlendFunc;

typedef void (*RendererAPIInitIMGUI_T)(void* renderer_instance, void* data, void* window);
inline static RendererAPIInitIMGUI_T RendererAPIInitIMGUI;

typedef void (*RendererAPIDestroyIMGUI_T)(void* data);
inline static RendererAPIDestroyIMGUI_T RendererAPIDestroyIMGUI;

typedef void (*RendererAPIIMGUIBegin_T)(void* data);
inline static RendererAPIIMGUIBegin_T RendererAPIIMGUIBegin;

typedef void (*RendererAPIIMGUIEnd_T)(void* data, void* drawData);
inline static RendererAPIIMGUIEnd_T RendererAPIIMGUIEnd;

//Shader
typedef void (*ShaderLoad_T)(void** data, const char* path);
inline static ShaderLoad_T ShaderLoad;

typedef void (*ShaderDestroy_T)(void** data);
inline static ShaderDestroy_T ShaderDestroy;

typedef void (*ShaderBind_T)(void* data);
inline static ShaderBind_T ShaderBind;

typedef void (*ShaderReload_T)(void* data, int recompile);
inline static ShaderReload_T ShaderReload;

typedef unsigned int (*ShaderGetID_T)(void* data);
inline static ShaderGetID_T ShaderGetID;

typedef char* (*ShaderGetName_T)(void* data);
inline static ShaderGetName_T ShaderGetName;

typedef char* (*ShaderGetPath_T)(void* data);
inline static ShaderGetPath_T ShaderGetPath;

typedef void (*ShaderSetUniformI_T)(void* data, const char* name, int value);
inline static ShaderSetUniformI_T ShaderSetUniformI;

typedef void (*ShaderSetUniformUI_T)(void* data, const char* name, unsigned int value);
inline static ShaderSetUniformUI_T ShaderSetUniformUI;

typedef void (*ShaderSetUniformF_T)(void* data, const char* name, float value);
inline static ShaderSetUniformF_T ShaderSetUniformF;

typedef void (*ShaderSetUniform2F_T)(void* data, const char* name, float x, float y);
inline static ShaderSetUniform2F_T ShaderSetUniform2F;

typedef void (*ShaderSetUniform3F_T)(void* data, const char* name, float x, float y, float z);
inline static ShaderSetUniform3F_T ShaderSetUniform3F;

typedef void (*ShaderSetUniform4F_T)(void* data, const char* name, float x, float y, float z, float w);
inline static ShaderSetUniform4F_T ShaderSetUniform4F;

typedef void (*ShaderSetUniformMat4_T)(void* data, const char* name, void* matrix);
inline static ShaderSetUniformMat4_T ShaderSetUniformMat4;

typedef void (*ShaderSetUniformMat3_T)(void* data, const char* name, void* matrix);
inline static ShaderSetUniformMat3_T ShaderSetUniformMat3;

typedef void (*ShaderSetUniform2I_T)(void* data, const char* name, int x, int y);
inline static ShaderSetUniform2I_T ShaderSetUniform2I;

typedef void (*ShaderSetUniform3I_T)(void* data, const char* name, int x, int y, int z);
inline static ShaderSetUniform3I_T ShaderSetUniform3I;

typedef void (*ShaderSetUniform4I_T)(void* data, const char* name, int x, int y, int z, int w);
inline static ShaderSetUniform4I_T ShaderSetUniform4I;
#pragma once
#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Renderer/Shared/VertexLayout.hpp>
#ifdef __cplusplus
namespace Engine
{
#endif
    EXPORT_RENDERER void RendererContextInit(void* handle);
    EXPORT_RENDERER void RendererContextDestroy(void* handle);
    EXPORT_RENDERER void RendererContextCreate(void* handle, void* window);
    EXPORT_RENDERER void RendererContextSwapBuffers(void* window);
    EXPORT_RENDERER void RendererContextAddDebugMessanger(void* window);
    EXPORT_RENDERER void RendererContextSetupWindowHints(void* window);

    EXPORT_RENDERER void CubemapTextureLoad(CubemapTextureData** data, void* cubemapName, void* paths);
    EXPORT_RENDERER void CubemapTextureBind(CubemapTextureData* data, unsigned int slot);
    EXPORT_RENDERER void CubemapTextureDestroy(CubemapTextureData** data);
    EXPORT_RENDERER CubemapTextureSpec* CubemapTextureGetSpec(CubemapTextureData* data);
    EXPORT_RENDERER int CubemapTextureGetID(CubemapTextureData* data);

    EXPORT_RENDERER void FramebufferInit(void** data, unsigned int width, unsigned int height, unsigned int isDepthMap);
    EXPORT_RENDERER int FramebufferGetID(void* data);
    EXPORT_RENDERER int FramebufferGetColorTextureID(void* data);
    EXPORT_RENDERER int FramebufferGetDepthTextureID(void* data);
    EXPORT_RENDERER void FramebufferBind(void* data);
    EXPORT_RENDERER void FramebufferBindColorTexture(void* data, uint32_t slot);
    EXPORT_RENDERER void FramebufferBindDepthTexture(void* data, uint32_t slot);
    EXPORT_RENDERER void FramebufferClearColor(void* data, void* color);
    EXPORT_RENDERER void FramebufferClearDepth(void* data);
    EXPORT_RENDERER void FramebufferUnbind(void* data);
    EXPORT_RENDERER void FramebufferDestroy(void** data);
    EXPORT_RENDERER unsigned int FramebufferGetWidth(void* data);
    EXPORT_RENDERER unsigned int FramebufferGetHeight(void* data);

    EXPORT_RENDERER void GUIContextInit(void** data, void* window);
    EXPORT_RENDERER void GUIContextDestroy(void** data);
    EXPORT_RENDERER void GUIContextBegin(void* data);
    EXPORT_RENDERER void GUIContextEnd(void* data, void* drawData);

    EXPORT_RENDERER void ImageInit(void** data, uint8_t* imageData, size_t width, size_t height, uint8_t type);
    EXPORT_RENDERER void ImageBind(void* data, size_t location);
    EXPORT_RENDERER void ImageDestroy(void** data);
    EXPORT_RENDERER uint32_t ImageGetID(void* data);


    EXPORT_RENDERER void IndexBufferInit(void** data, void* vertexArray, const uint32_t* indexData, uint32_t length);
    EXPORT_RENDERER void IndexBufferDestroy(void** data);
    EXPORT_RENDERER void IndexBufferBind(void* data);
    EXPORT_RENDERER uint32_t IndexBufferGetID(void* data);
    EXPORT_RENDERER uint32_t IndexBufferGetSize(void* data);
    EXPORT_RENDERER uint32_t IndexBufferGetLength(void* data);

    EXPORT_RENDERER void RendererAPISetClearColor(void* data, float r, float g, float b, float a);
    EXPORT_RENDERER void RendererAPIInit(void** data, void* rendererSpec, void* applicationSpec);
    EXPORT_RENDERER void RendererAPIDestroy(void** data);
    EXPORT_RENDERER void RendererAPISwitchPolygonMode(void* data, unsigned int mode);
    EXPORT_RENDERER void RendererAPISetViewport(void* data, float width, float height);
    EXPORT_RENDERER void RendererAPIBindDefaultFramebuffer();
    EXPORT_RENDERER void RendererAPIRenderIndexed(void* data, uint32_t indexCount);
    EXPORT_RENDERER void RendererAPIChangeDepthFunc(void* data, unsigned int depthFunction);
    EXPORT_RENDERER void RendererAPIInitIMGUI(void* renderer_instance, void* data, void* window);
    EXPORT_RENDERER void RendererAPIDestroyIMGUI(void* data);
    EXPORT_RENDERER void RendererAPIIMGUIBegin(void* data);
    EXPORT_RENDERER void RendererAPIIMGUIEnd(void* data, void* drawData);

    EXPORT_RENDERER void ShaderReload(void* data, int recompile);
    EXPORT_RENDERER void ShaderLoad(void** data, const char* path);
    EXPORT_RENDERER void ShaderBind(void* data);
    EXPORT_RENDERER unsigned int ShaderGetID(void* data);
    EXPORT_RENDERER void ShaderDestroy(void** data);
    EXPORT_RENDERER void ShaderSetUniformF(void* data, const char* fullname, float value);
    EXPORT_RENDERER void ShaderSetUniformI(void* data, const char* fullname, int32_t value);
    EXPORT_RENDERER void ShaderSetUniformUI(void* data, const char* fullname, uint32_t value);
    EXPORT_RENDERER void ShaderSetUniform2F(void* data, const char* fullname, float x, float y);
    EXPORT_RENDERER void ShaderSetUniform3F(void* data, const char* fullname, float x, float y, float z);
    EXPORT_RENDERER void ShaderSetUniform4F(void* data, const char* fullname, float x, float y, float z, float w);
    EXPORT_RENDERER void ShaderSetUniform2I(void* data, const char* fullname, int32_t x, int32_t y);
    EXPORT_RENDERER void ShaderSetUniform3I(void* data, const char* fullname, int32_t x, int32_t y, int32_t z);
    EXPORT_RENDERER void ShaderSetUniform4I(void* data, const char* fullname, int32_t x, int32_t y, int32_t z,
                                            int32_t w);
    EXPORT_RENDERER void ShaderSetUniformMat3(void* data, const char* fullname, void* matrix);
    EXPORT_RENDERER void ShaderSetUniformMat4(void* data, const char* fullname, void* matrix);
    EXPORT_RENDERER const char* ShaderGetName(void* data);
    EXPORT_RENDERER const char* ShaderGetPath(void* data);


    EXPORT_RENDERER void StorageBufferInit(void** data, void* vertexArray, char* storageData, unsigned int size,
                                           int type);
    EXPORT_RENDERER void StorageBufferBind(void* data, unsigned int location);
    EXPORT_RENDERER void StorageBufferUnbind(void* data);
    EXPORT_RENDERER void StorageBufferDestroy(void** data);
    EXPORT_RENDERER uint32_t StorageBufferGetID(void* data);

    EXPORT_RENDERER void TextureArrayLoad(void** data, const char* textureName, char** paths, unsigned int count);
    EXPORT_RENDERER void TextureArrayBind(void* data, uint32_t slot);
    EXPORT_RENDERER void TextureArrayDestroy(void** data);
    EXPORT_RENDERER char* TextureArrayGetName(void* data);
    EXPORT_RENDERER uint32_t TextureArrayGetID(void* data);
    EXPORT_RENDERER uint32_t TextureArrayGetWidth(void* data);
    EXPORT_RENDERER uint32_t TextureArrayGetHeight(void* data);
    EXPORT_RENDERER uint32_t TextureArrayGetChannels(void* data);
    EXPORT_RENDERER uint32_t TextureArrayGetCount(void* data);

    EXPORT_RENDERER VertexArrayData* VertexArrayInit(uint32_t indexCount);
    EXPORT_RENDERER void VertexArrayBind(VertexArrayData* data);
    EXPORT_RENDERER void VertexArrayUnbind(VertexArrayData* data);
    EXPORT_RENDERER void VertexArrayDestroy(VertexArrayData** data);
    EXPORT_RENDERER void VertexArrayAddVertexBuffer(VertexArrayData* data, VertexLayout* layout, float* vertexData,
                                                    unsigned int length);
    EXPORT_RENDERER void VertexArrayAddIndexBuffer(VertexArrayData* data, unsigned int* indexData, unsigned int length);
    EXPORT_RENDERER uint32_t VertexArrayGetIndexCount(VertexArrayData* data);
    EXPORT_RENDERER uint32_t VertexArrayGetID(VertexArrayData* data);

    EXPORT_RENDERER VertexBufferData* VertexBufferInit(VertexArrayData* vertexArray, VertexLayout* vertexLayout,
                                                       float* vertices, unsigned int length);
    EXPORT_RENDERER void VertexBufferBind(VertexBufferData* data);
    EXPORT_RENDERER size_t VertexBufferGetSize(VertexBufferData* data);
    EXPORT_RENDERER void VertexBufferDestroy(VertexBufferData** data);
    EXPORT_RENDERER uint32_t VertexBufferGetID(VertexBufferData* data);
#ifdef __cplusplus
}
#endif
#pragma once
#include <cstdint>
#include <Renderer/Shared/StorageBufferType.hpp>
#include <Renderer/Shared/CubemapTextureSpec.hpp>
#ifdef __cplusplus
namespace Engine
{
#endif

    struct ImageData {
        uint32_t id;
        uint8_t format;
        uint8_t type;
    };

    struct FramebufferData {
        uint32_t width{};
        uint32_t height{};
        uint32_t fbo{};
        ImageData* color_attachment_data{};
        ImageData* depth_attachment_data{};
    };

    struct VertexBufferData {
        uint32_t id{};
        uint32_t size{};
    };

    struct IndexBufferData {
        uint32_t id{};
        uint32_t count{};
        uint32_t stride{};
    };

    struct VertexArrayData {
        uint32_t id{};
        uint32_t indexCount{};
        VertexBufferData* vertexBufferData{};
        IndexBufferData* indexBufferData{};
    };

    struct TextureArrayData {
        uint32_t id{};
        uint32_t width{};
        uint32_t height{};
        uint32_t channels{};
        uint32_t count{};
        char* name{};
    };

    struct StorageBufferData {
        uint32_t id{};
        uint32_t stride{};
        uint32_t count{};
        StorageBufferType type{};
    };

    struct DrawIndirectBufferData {
        uint32_t id{};
        uint32_t stride{};
        uint32_t count{};
        StorageBufferType type{};
    };

    struct RendererAPIData {
        void* guiContext;
    };

    struct ShaderData {
        uint32_t rendererID = 0;
        bool loaded = false;
        bool compiled = false;

        char* name;
        char* assetPath;
        // std::string name, assetPath;

        // std::unordered_map<GLenum, std::string> shaderSource;
        void* shaderSource;

        void* uniformBlocks;
        // std::vector<ShaderUniformBlockLayout> uniformBlocks;
    };

    struct CubemapTextureData {
        CubemapTextureSpec spec{};
        uint32_t id{};
    };
#ifdef __cplusplus
}// namespace Engine
#endif
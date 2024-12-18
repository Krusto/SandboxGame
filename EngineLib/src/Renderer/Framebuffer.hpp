#pragma once
#include <Renderer/Viewport.hpp>
#include <Renderer/RendererCommand.hpp>
#include <Renderer/Predefines.hpp>

#include <glm/glm.hpp>

namespace Engine
{
    struct FramebufferData;

    class EXPORT_RENDERER Framebuffer
    {
    public:
        Framebuffer() = default;
        ~Framebuffer() = default;
        static Framebuffer Create(uint32_t width, uint32_t height, bool isDepthMap = false);

    public:
        RendererCommand BindCommand() const;
        RendererCommand BindColorTextureCommand(uint32_t slot) const;
        RendererCommand BindDepthTextureCommand(uint32_t slot) const;
        RendererCommand ClearColorCommand(glm::vec4 color) const;
        RendererCommand ClearDepthCommand() const;

    public:
        void Init(uint32_t width, uint32_t height, bool isDepthMap = false);
        uint32_t GetID();
        uint32_t GetColorAttachmentID();
        uint32_t GetDepthAttachmentID();
        void Resize(uint32_t width, uint32_t height);
        void Destroy();
        void Bind() const;
        void BindColorTexture(uint32_t slot) const;
        void BindDepthTexture(uint32_t slot) const;
        void ClearColor(glm::vec4 color) const;
        void ClearDepth() const;
        void Unbind();
        uint32_t width();
        uint32_t height();
        ViewportSize GetViewportSize();

    private:
        FramebufferData* m_Data{};
    };
}// namespace Engine
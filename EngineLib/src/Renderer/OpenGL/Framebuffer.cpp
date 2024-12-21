#include <glad/glad.h>
#include <Core/Allocator.hpp>
#include <Renderer/Shared/APISpecific/Image.hpp>
#include <Renderer/Shared/APISpecific/Framebuffer.hpp>

namespace Engine
{

    struct FramebufferData {
        uint32_t width{};
        uint32_t height{};
        uint32_t fbo{};
        Image color_attachment{};
        Image depth_attachment{};
    };

    void Framebuffer::Init(uint32_t width, uint32_t height, bool isDepthMap)
    {
        if (m_Data == nullptr) { m_Data = Engine::Allocator::Allocate<FramebufferData>(); }
        m_Data->width = width;
        m_Data->height = height;

        glGenFramebuffers(1, &m_Data->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Data->fbo);

        if (!isDepthMap)
        {
            m_Data->color_attachment = Image::Create(nullptr, width, height, ImageType::RGB);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                                   m_Data->color_attachment.GetID(), 0);

            m_Data->depth_attachment = Image::Create(nullptr, width, height, ImageType::Depth);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Data->depth_attachment.GetID(),
                                   0);
        }
        else
        {

            m_Data->depth_attachment = Image::Create(nullptr, width, height, ImageType::Depth);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_Data->depth_attachment.GetID(),
                                   0);

            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t Framebuffer::GetID() { return m_Data->fbo; }

    uint32_t Framebuffer::GetColorAttachmentID() { return m_Data->color_attachment.GetID(); }

    uint32_t Framebuffer::GetDepthAttachmentID() { return m_Data->depth_attachment.GetID(); }

    void Framebuffer::Resize(uint32_t width, uint32_t height)
    {
        Destroy();
        Init(width, height);
    }

    void Framebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_Data->fbo);
        glViewport(0, 0, (GLsizei) m_Data->width, (GLsizei) m_Data->height);
    }

    void Framebuffer::BindColorTexture(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_Data->color_attachment.GetID());
    }

    void Framebuffer::BindDepthTexture(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_Data->depth_attachment.GetID());
    }

    void Framebuffer::ClearColor(glm::vec4 color) const
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void Framebuffer::ClearDepth() const { glClear(GL_DEPTH_BUFFER_BIT); }

    void Framebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void Framebuffer::Destroy()
    {
        if (m_Data == nullptr) { return; }

        m_Data->color_attachment.Destroy();
        m_Data->depth_attachment.Destroy();
        glDeleteFramebuffers(1, &m_Data->fbo);

        Allocator::Deallocate(m_Data);
        m_Data = nullptr;
    }

    uint32_t Framebuffer::width() { return m_Data->width; }

    uint32_t Framebuffer::height() { return m_Data->height; }

    ViewportSize Framebuffer::GetViewportSize() { return {m_Data->width, m_Data->height}; }
}// namespace Engine
#include <glad/glad.h>
#include <Core/Allocator.hpp>
#include "OpenGLFramebuffer.hpp"

namespace Engine
{

    void OpenGLFramebuffer::Init(uint32_t width, uint32_t height, bool isDepthMap)
    {
        this->m_width = width;
        this->m_height = height;

        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        if (!isDepthMap)
        {
            m_color_attachment = Image::Create(nullptr, width, height, ImageType::RGB);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_attachment->GetID(), 0);

            m_depth_attachment = Image::Create(nullptr, width, height, ImageType::Depth);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment->GetID(), 0);
        }
        else
        {

            m_depth_attachment = Image::Create(nullptr, width, height, ImageType::Depth);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_attachment->GetID(), 0);

            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFramebuffer::GetID() { return m_fbo; }

    uint32_t OpenGLFramebuffer::GetColorAttachmentID() { return m_color_attachment->GetID(); }

    uint32_t OpenGLFramebuffer::GetDepthAttachmentID() { return m_depth_attachment->GetID(); }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        Destroy();
        Init(width, height);
    }

    void OpenGLFramebuffer::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glViewport(0, 0, (GLsizei) m_width, (GLsizei) m_height);
    }

    void OpenGLFramebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void OpenGLFramebuffer::Destroy()
    {
        if (m_color_attachment)
        {
            m_color_attachment->Destroy();
            Allocator::Deallocate(m_color_attachment);
            m_color_attachment = nullptr;
        }
        if (m_depth_attachment)
        {
            m_depth_attachment->Destroy();
            Allocator::Deallocate(m_depth_attachment);
            m_depth_attachment = nullptr;
        }
        glDeleteFramebuffers(1, &m_fbo);
    }

    uint32_t OpenGLFramebuffer::width() { return m_width; }

    uint32_t OpenGLFramebuffer::height() { return m_height; }

    ViewportSize OpenGLFramebuffer::GetViewportSize() { return {m_width, m_height}; }
}// namespace Engine
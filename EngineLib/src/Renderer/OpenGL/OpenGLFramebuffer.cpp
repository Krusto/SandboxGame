#include <glad/glad.h>

#include "OpenGLFramebuffer.hpp"

namespace Engine
{

    void OpenGLFramebuffer::Init(uint32_t width, uint32_t height)
    {
        this->m_width = width;
        this->m_height = height;

        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        glGenTextures(1, &m_color_texture);
        glBindTexture(GL_TEXTURE_2D, m_color_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) width, (GLsizei) height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color_texture, 0);

        glGenTextures(1, &m_depth_texture);
        glBindTexture(GL_TEXTURE_2D, m_depth_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (GLsizei) width, (GLsizei) height, 0, GL_DEPTH_COMPONENT,
                     GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    uint32_t OpenGLFramebuffer::GetID() { return m_fbo; }

    uint32_t& OpenGLFramebuffer::GetColorAttachmentID() { return m_color_texture; }

    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        Destroy();
        Init(width, height);
    }

    void OpenGLFramebuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glViewport(0, 0, (GLsizei) m_width, (GLsizei) m_height);
    }

    void OpenGLFramebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void OpenGLFramebuffer::Destroy()
    {
        glDeleteTextures(1, &m_color_texture);
        glDeleteTextures(1, &m_depth_texture);
        glDeleteFramebuffers(1, &m_fbo);
    }

    uint32_t OpenGLFramebuffer::width() { return m_width; }

    uint32_t OpenGLFramebuffer::height() { return m_height; }

    ViewportSize OpenGLFramebuffer::GetViewportSize() { return {m_width, m_height}; }
}// namespace Engine
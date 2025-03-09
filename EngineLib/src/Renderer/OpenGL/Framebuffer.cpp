#include <glad/glad.h>
#include <glm/vec4.hpp>
#include <Core/Allocator.hpp>
#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Renderer/OpenGL/ExportedFunctions.hpp>
#include <Renderer/Shared/ImageType.hpp>
#include <Renderer/Shared/Viewport.hpp>

namespace Engine
{

    EXPORT_RENDERER void FramebufferInit(void** data, unsigned int width, unsigned int height, unsigned int isDepthMap)
    {
        if (*data == nullptr) { *data = Engine::Allocator::Allocate<FramebufferData>(); }

        FramebufferData* m_Data = (FramebufferData*) *data;
        m_Data->width = width;
        m_Data->height = height;

        glCreateFramebuffers(1, &m_Data->fbo);
        if (!isDepthMap)
        {
            HDRImageInit((void**) &m_Data->color_attachment_data, nullptr, width, height,
                         (uint8_t) ImageColorFormat::RGB, (uint8_t) ImageType::DYNAMIC);
            glNamedFramebufferTexture(m_Data->fbo, GL_COLOR_ATTACHMENT0, m_Data->color_attachment_data->id, 0);

            ImageInit((void**) &m_Data->depth_attachment_data, nullptr, width, height,
                      (uint8_t) ImageColorFormat::Depth, (uint8_t) ImageType::DYNAMIC);
            glNamedFramebufferTexture(m_Data->fbo, GL_DEPTH_ATTACHMENT, m_Data->depth_attachment_data->id, 0);
        }
        else
        {
            ImageInit((void**) &m_Data->depth_attachment_data, nullptr, width, height,
                      (uint8_t) ImageColorFormat::Depth, (uint8_t) ImageType::DYNAMIC);
            glNamedFramebufferTexture(m_Data->fbo, GL_DEPTH_ATTACHMENT, m_Data->depth_attachment_data->id, 0);
        }
        if (glCheckNamedFramebufferStatus(m_Data->fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            LOG_ERROR("Framebuffer not complete!\n");
        }
    }

    EXPORT_RENDERER int FramebufferGetID(void* data) { return asTPtr(data, FramebufferData)->fbo; }

    EXPORT_RENDERER int FramebufferGetColorTextureID(void* data)
    {
        return asTPtr(asTPtr(data, FramebufferData)->color_attachment_data, ImageData)->id;
    }

    EXPORT_RENDERER int FramebufferGetDepthTextureID(void* data)
    {
        return asTPtr(asTPtr(data, FramebufferData)->depth_attachment_data, ImageData)->id;
    }

    EXPORT_RENDERER void FramebufferBind(void* data)
    {
        FramebufferData* m_Data = (FramebufferData*) data;
        glBindFramebuffer(GL_FRAMEBUFFER, m_Data->fbo);
        glViewport(0, 0, (GLsizei) m_Data->width, (GLsizei) m_Data->height);
    }

    EXPORT_RENDERER void FramebufferBindColorTexture(void* data, uint32_t slot)
    {
        FramebufferData* m_Data = (FramebufferData*) data;
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, ImageGetID(m_Data->color_attachment_data));
    }

    EXPORT_RENDERER void FramebufferBindDepthTexture(void* data, uint32_t slot)
    {
        FramebufferData* m_Data = (FramebufferData*) data;

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, ImageGetID(m_Data->depth_attachment_data));
    }

    EXPORT_RENDERER void FramebufferClearColor(void* data, void* color)
    {
        FramebufferData* m_Data = (FramebufferData*) data;
        glm::vec4* color_ = ((glm::vec4*) color);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(color_->r, color_->g, color_->b, color_->a);
    }

    EXPORT_RENDERER void FramebufferClearDepth(void* data) { glClear(GL_DEPTH_BUFFER_BIT); }

    EXPORT_RENDERER void FramebufferUnbind(void* data) { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    EXPORT_RENDERER void FramebufferDestroy(void** data)
    {
        FramebufferData* m_Data = (FramebufferData*) *data;
        if (m_Data == nullptr) { return; }

        ImageDestroy((void**) &m_Data->color_attachment_data);
        ImageDestroy((void**) &m_Data->depth_attachment_data);
        glDeleteFramebuffers(1, &m_Data->fbo);

        Allocator::Deallocate(m_Data);
        *data = nullptr;
    }

    EXPORT_RENDERER unsigned int FramebufferGetWidth(void* data)
    {
        FramebufferData* m_Data = (FramebufferData*) data;
        return m_Data->width;
    }

    EXPORT_RENDERER unsigned int FramebufferGetHeight(void* data)
    {
        FramebufferData* m_Data = (FramebufferData*) data;
        return m_Data->height;
    }

}// namespace Engine
#include "OpenGLRenderer.hpp"

namespace Engine
{
    void OpenGLRenderer::ClearColor(glm::vec4 color)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderer::Init(RendererSpec rendererSpec, ApplicationSpec applicationSpec) {}

    void OpenGLRenderer::Shutdown(){};
    void OpenGLRenderer::BeginFrame(){};
    void OpenGLRenderer::EndFrame(){};

    void OpenGLRenderer::SwitchMode(uint32_t mode)
    {
        if (mode == 0) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
        else if (mode == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    }

    void OpenGLRenderer::SetViewport(ViewportSize size) { glViewport(0, 0, size.width, size.height); }

    void OpenGLRenderer::BindDefaultFramebuffer() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void OpenGLRenderer::RenderIndexed(VertexArray* vertexArray, uint32_t indexCount) const
    {
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    };

}// namespace Engine
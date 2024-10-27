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
}// namespace Engine
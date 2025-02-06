#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Core/Allocator.hpp>
#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Renderer/Shared/DepthFunction.hpp>
#include <Renderer/Renderer.hpp>
namespace Engine
{

    EXPORT_RENDERER void RendererAPISetClearColor(void* data, float r, float g, float b, float a)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(r, g, b, a);
    }

    EXPORT_RENDERER void RendererAPIInit(void** data, void* rendererSpec, void* applicationSpec)
    {
        RendererAPIData* m_Data = static_cast<RendererAPIData*>(*data);
        if (m_Data == nullptr) { m_Data = Allocator::Allocate<RendererAPIData>(); }
    }

    EXPORT_RENDERER void RendererAPIDestroy(void** data)
    {
        RendererAPIData* m_Data = static_cast<RendererAPIData*>(*data);
        if (m_Data == nullptr) { return; }
        Renderer::s_Renderer->GUIContextDestroy((void**) &m_Data->guiContext);
        Allocator::Deallocate(m_Data);
        *data = nullptr;
    };

    EXPORT_RENDERER void RendererAPISwitchPolygonMode(void* data, unsigned int mode)
    {

        if (mode == 0) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
        else if (mode == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    }

    EXPORT_RENDERER void RendererAPISetViewport(void* data, float width, float height)
    {
        glViewport(0, 0, width, height);
    }

    EXPORT_RENDERER void RendererAPIBindDefaultFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    EXPORT_RENDERER void RendererAPIRenderIndexed(void* data, uint32_t indexCount)
    {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    };

    EXPORT_RENDERER void RendererAPIChangeDepthFunc(void* data, unsigned int depthFunction)
    {
        switch (depthFunction)
        {
            case DepthFunction::Never:
                glDepthFunc(GL_NEVER);
                break;
            case DepthFunction::Less:
                glDepthFunc(GL_LESS);
                break;
            case DepthFunction::Equal:
                glDepthFunc(GL_EQUAL);
                break;
            case DepthFunction::LEqual:
                glDepthFunc(GL_LEQUAL);
                break;
            case DepthFunction::Greater:
                glDepthFunc(GL_GREATER);
                break;
            case DepthFunction::NotEqual:
                glDepthFunc(GL_NOTEQUAL);
                break;
            case DepthFunction::GEqual:
                glDepthFunc(GL_GEQUAL);
                break;
            case DepthFunction::Always:
                glDepthFunc(GL_ALWAYS);
                break;
        }
    }

    EXPORT_RENDERER void RendererAPIInitIMGUI(void* renderer_instance,void* data, void* window)
    {
        RendererAPIData* m_Data = static_cast<RendererAPIData*>(data);
        ((Engine::Renderer*)renderer_instance)->GUIContextInit((void**) &m_Data->guiContext, window);
    }

    EXPORT_RENDERER void RendererAPIDestroyIMGUI(void* data)
    {
        RendererAPIData* m_Data = static_cast<RendererAPIData*>(data);
        Renderer::s_Renderer->GUIContextDestroy((void**) &m_Data->guiContext);
    }

    EXPORT_RENDERER void RendererAPIIMGUIBegin(void* data)
    {
        RendererAPIData* m_Data = static_cast<RendererAPIData*>(data);
        Renderer::s_Renderer->GUIContextBegin(&m_Data->guiContext);
    }

    EXPORT_RENDERER void RendererAPIIMGUIEnd(void* data, void* drawData)
    {
        RendererAPIData* m_Data = static_cast<RendererAPIData*>(data);
        Renderer::s_Renderer->GUIContextEnd(&m_Data->guiContext, drawData);
    }
}// namespace Engine

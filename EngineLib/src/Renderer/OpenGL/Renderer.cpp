#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Core/Allocator.hpp>
#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Renderer/Shared/DepthFunction.hpp>
#include <Renderer/Renderer.hpp>
#include "ExportedFunctions.hpp"

namespace Engine
{

    EXPORT_RENDERER void RendererAPISetClearColor(RendererAPIData* data, float r, float g, float b, float a)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(r, g, b, a);
    }

    EXPORT_RENDERER void RendererAPIInit(RendererAPIData** data, void* rendererSpec, void* applicationSpec)
    {
        if (data == nullptr) { *data = Allocator::Allocate<RendererAPIData>(); }
    }

    EXPORT_RENDERER void RendererAPIDestroy(RendererAPIData** data)
    {
        if (data == nullptr) { return; }
        GUIContextDestroy((void**) &(*data)->guiContext);
        Allocator::Deallocate(data);
        *data = nullptr;
    };

    EXPORT_RENDERER void RendererAPISwitchPolygonMode(RendererAPIData* data, unsigned int mode)
    {

        if (mode == 0) { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
        else if (mode == 1) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    }

    EXPORT_RENDERER void RendererAPISetViewport(RendererAPIData* data, float width, float height)
    {
        glViewport(0, 0, width, height);
    }

    EXPORT_RENDERER void RendererAPIBindDefaultFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    EXPORT_RENDERER void RendererAPIRenderIndexed(RendererAPIData* data, uint32_t indexCount)
    {
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    };

    EXPORT_RENDERER void RendererAPIChangeDepthFunc(RendererAPIData* data, unsigned int depthFunction)
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

    EXPORT_RENDERER void RendererAPIInitIMGUI(void* renderer_instance, RendererAPIData* data, void* window)
    {
        GUIContextInit((void**) &data->guiContext, window);
    }

    EXPORT_RENDERER void RendererAPIDestroyIMGUI(RendererAPIData* data)
    {
        GUIContextDestroy((void**) &data->guiContext);
    }

    EXPORT_RENDERER void RendererAPIIMGUIBegin(RendererAPIData* data) { GUIContextBegin(&data->guiContext); }

    EXPORT_RENDERER void RendererAPIIMGUIEnd(RendererAPIData* data, void* drawData)
    {
        GUIContextEnd(&data->guiContext, drawData);
    }
}// namespace Engine

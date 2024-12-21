#pragma once
#include <Renderer/Predefines.hpp>
typedef struct GLFWwindow GLFWwindow;
typedef struct ImDrawData ImDrawData;

namespace Engine
{

    class EXPORT_RENDERER GUIContext
    {
    public:
        GUIContext() = default;
        ~GUIContext() = default;

    public:
        void Init(GLFWwindow* window);

        void Shutdown();

        void NewFrame();

        void Render(ImDrawData* drawData);

    private:
        void _RendererSpecificInit(GLFWwindow* window);
        void _RendererSpecificShutdown();
        void _RendererSpecificNewFrame();
        void _RendererSpecificRender(ImDrawData* drawData);
    };
}// namespace Engine
#pragma once
#include <Renderer/Predefines.hpp>
typedef struct GLFWwindow GLFWwindow;
typedef struct ImDrawData ImDrawData;

namespace Engine
{

    class GUIContext
    {
    public:
        GUIContext() = default;
        ~GUIContext() = default;

    public:
        void Init(GLFWwindow* window);

        void Shutdown();

        void NewFrame();

        void Render(ImDrawData* drawData);
    };
}// namespace Engine
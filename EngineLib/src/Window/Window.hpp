//
// Created by Stoyanov, Krusto (K.S.) on 6/3/2022.
//

#pragma once

#include <Renderer/Renderer.hpp>
#include <Application/ApplicationSpec.hpp>
#include <Core/Predefines.hpp>
#include <Core/Log.hpp>
#include <Core/Ref.hpp>
#include <Layer/LayerStack.hpp>
#include <Util/Version.hpp>

#include <filesystem>

namespace Engine
{
    class Window;
    typedef struct GLFWwindow GLFWwindow;

    struct WindowSpec {
        std::string_view title;
        uint32_t width;
        uint32_t height;
    };

    class EXPORT_ENGINE Window: public RefCounted
    {
    public:
        Window() = default;
        ~Window() = default;

    public:
        void Create(WindowSpec spec);

        void Destroy();

        void Update();

        bool IsValid();

        void Close();

        bool ShouldClose();

        double BeginFrameTime() const;
        double* BeginFrameTime();
        double EndFrameTime() const;
        double* EndFrameTime();
        double DeltaFrameTime() const;

        GLFWwindow* GetRawHandler();

        WindowSpec* GetSpec();
        const WindowSpec* GetSpec() const;

        GLFWwindow* GetCurrentContext() const;
        void SetCurrentContext(GLFWwindow* context);

    private:
        static void _CloseCallback(GLFWwindow* window);
        static void _WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void _WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void _WindowMouseMoveCallback(GLFWwindow* window, double x, double y);
        static void _WindowMouseScrollCallback(GLFWwindow* window, double x, double y);
        static void _ErrorCallback(int code, const char* err_str);
        static void _WindowMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    private:
        WindowSpec m_WindowSpec{};
        double m_BeginFrameTime{};
        double m_EndFrameTime{};
    };
}// namespace Engine

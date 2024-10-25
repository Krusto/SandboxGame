//
// Created by Stoyanov, Krusto (K.S.) on 6/3/2022.
//

#pragma once

#include <Core/Log.hpp>
#include <Renderer/Viewport.hpp>
#include <Layer/LayerStack.hpp>
#include <Util/Version.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <filesystem>

namespace Engine
{
    class Window;

    struct ApplicationSpec {
        std::string_view ApplicationName;
        std::filesystem::path WorkingDirectory;
        Engine::Version AppVersion;
        uint32_t width;
        uint32_t height;
    };

    struct WindowSpec {
        std::string_view title;
        uint32_t width;
        uint32_t height;
    };

    class Window
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

    public:
        static inline ViewportSize s_ViewportSize{};

    private:
        static void _CloseCallback(GLFWwindow* window);
        static void _WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void _WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void _WindowMouseMoveCallback(GLFWwindow* window, double x, double y);

    private:
        static inline GLFWwindow* s_WindowPtr = nullptr;

        WindowSpec m_WindowSpec{};
        double m_BeginFrameTime{};
        double m_EndFrameTime{};
    };
}// namespace Engine
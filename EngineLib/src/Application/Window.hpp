//
// Created by Stoyanov, Krusto (K.S.) on 6/3/2022.
//

#pragma once

#include <Application/Predefines.hpp>
#include <Application/ApplicationSpec.hpp>
#include <Core/Log.hpp>
#include <Core/Ref.hpp>
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

    struct WindowSpec {
        std::string_view title;
        uint32_t width;
        uint32_t height;
    };

    class EXPORT_ENGINE_ENTRY Window: public RefCounted
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


    private:
        static void _CloseCallback(GLFWwindow* window);
        static void _WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void _WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void _WindowMouseMoveCallback(GLFWwindow* window, double x, double y);
        static void _WindowMouseScrollCallback(GLFWwindow* window, double x, double y);

        static void GLAPIENTRY _MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                                GLchar const* message, void const* user_param);

        static void _ErrorCallback(int code, const char* err_str);

    private:
        WindowSpec m_WindowSpec{};
        double m_BeginFrameTime{};
        double m_EndFrameTime{};
    };
}// namespace Engine
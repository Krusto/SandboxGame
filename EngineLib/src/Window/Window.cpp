#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Window.hpp"

#include <Renderer/Renderer.hpp>

#include <Core/Log.hpp>

EXPORT_ENGINE ViewportSize s_ViewportSize{};
EXPORT_ENGINE GLFWwindow* s_WindowPtr{};

namespace Engine
{
    void Window::Create(WindowSpec spec)
    {
        this->m_WindowSpec = spec;
        glfwSetErrorCallback(_ErrorCallback);

        if (!glfwInit())
        {
            LOG_ERROR("GLFW initialization failed!");
            exit(-1);
        }

        GraphicsContext::Get()->SetupWindowHints();

        int major, minor, rev;
        glfwGetVersion(&major, &minor, &rev);

        LOG_INFO("GLFW VERSION: %i.%i.%i\n", major, minor, rev);

        s_WindowPtr = glfwCreateWindow(this->m_WindowSpec.width, this->m_WindowSpec.height,
                                       this->m_WindowSpec.title.data(), nullptr, nullptr);
        s_ViewportSize.width = m_WindowSpec.width;
        s_ViewportSize.height = m_WindowSpec.height;

        if (!IsValid())
        {
            LOG_ERROR("Failed to create GLFW window!");
            glfwTerminate();
            exit(-1);
        }

        glfwMakeContextCurrent(s_WindowPtr);

        GraphicsContext::Get()->Create(s_WindowPtr);
        GraphicsContext::Get()->Init();
        GraphicsContext::Get()->AddDebugMessanger();

        glfwSetWindowCloseCallback(s_WindowPtr, _CloseCallback);
        glfwSetWindowSizeCallback(s_WindowPtr, _WindowSizeCallback);
        glfwSetKeyCallback(s_WindowPtr, _WindowKeyCallback);
        glfwSetCursorPosCallback(s_WindowPtr, _WindowMouseMoveCallback);
        glfwSetScrollCallback(s_WindowPtr, _WindowMouseScrollCallback);
        glfwSwapInterval(1);
    }

    void Window::Destroy()
    {
        glfwDestroyWindow(s_WindowPtr);
        GraphicsContext::Get()->Destroy();
        glfwTerminate();
    }

    void Window::Update()
    {
        GraphicsContext::Get()->SwapBuffers();
        m_WindowSpec.width = s_ViewportSize.width;
        m_WindowSpec.height = s_ViewportSize.height;
    }

    bool Window::IsValid() { return (s_WindowPtr != nullptr); }

    void Window::Close() { glfwSetWindowShouldClose(s_WindowPtr, GLFW_TRUE); }

    bool Window::ShouldClose() { return glfwWindowShouldClose(s_WindowPtr); }

    double Window::BeginFrameTime() const { return m_BeginFrameTime; }

    double* Window::BeginFrameTime() { return &m_BeginFrameTime; }

    double Window::EndFrameTime() const { return m_EndFrameTime; }

    double* Window::EndFrameTime() { return &m_EndFrameTime; }

    double Window::DeltaFrameTime() const { return (m_EndFrameTime - m_BeginFrameTime); }

    GLFWwindow* Window::GetRawHandler() { return s_WindowPtr; }

    WindowSpec* Window::GetSpec() { return &m_WindowSpec; }

    const WindowSpec* Window::GetSpec() const { return &m_WindowSpec; }

    GLFWwindow* Window::GetCurrentContext() const { return glfwGetCurrentContext(); }

    void Window::SetCurrentContext(GLFWwindow* context) { glfwMakeContextCurrent(context); }

    void Window::_CloseCallback(GLFWwindow* window)
    {
        for (auto& layer: LayerStack::data())
        {
            layer->OnWindowShouldCloseEvent();
            layer->SetShouldExit(true);
        }
    }

    void Window::_WindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        s_ViewportSize.width = width;
        s_ViewportSize.height = height;
        for (auto& layer: LayerStack::data()) { layer->OnWindowResizeEvent(width, height); }
    }

    void Window::_WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        for (auto& layer: LayerStack::data()) { layer->OnKeyboardEvent(action, key); }
    }

    void Window::_WindowMouseMoveCallback(GLFWwindow* window, double x, double y)
    {
        for (auto& layer: LayerStack::data()) { layer->OnMouseMoveEvent((int) x, (int) y); }
    }

    void Window::_WindowMouseScrollCallback(GLFWwindow* window, double x, double y)
    {
        for (auto& layer: LayerStack::data()) { layer->OnMouseScrollEvent(x, y); }
    }

    void Window::_ErrorCallback(int code, const char* err_str)
    {
        if (std::string(err_str) != "Invalid window attribute 0x0002000D")
            LOG_ERROR("%i GLFW Error: %s", code, err_str);
    }

}// namespace Engine
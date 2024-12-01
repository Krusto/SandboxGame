#include "Window.hpp"

#include "GraphicsContext.hpp"
#include "Renderer.hpp"

#include <Core/Log.hpp>

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

        constexpr auto OPENGL_VERSION_MAJOR = 4;
        constexpr auto OPENGL_VERSION_MINOR = 6;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        int major, minor, rev;
        glfwGetVersion(&major, &minor, &rev);

        LOG_INFO("GLFW VERSION: %i.%i.%i\n", major, minor, rev);

        s_WindowPtr = glfwCreateWindow(this->m_WindowSpec.width, this->m_WindowSpec.height,
                                       this->m_WindowSpec.title.data(), nullptr, nullptr);
        if (!IsValid())
        {
            LOG_ERROR("Failed to create GLFW window!");
            glfwTerminate();
            exit(-1);
        }

        glfwMakeContextCurrent(s_WindowPtr);

        GraphicsContext::s_Context = GraphicsContext::Create(s_WindowPtr);
        GraphicsContext::s_Context->Init();

        if constexpr (OPENGL_VERSION_MAJOR == 4 && OPENGL_VERSION_MINOR >= 3)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(Engine::Window::_MessageCallback, 0);
        }
        glfwSetWindowCloseCallback(this->s_WindowPtr, _CloseCallback);
        glfwSetWindowSizeCallback(this->s_WindowPtr, _WindowSizeCallback);
        glfwSetKeyCallback(this->s_WindowPtr, _WindowKeyCallback);
        glfwSetCursorPosCallback(this->s_WindowPtr, _WindowMouseMoveCallback);
        glfwSetScrollCallback(this->s_WindowPtr, _WindowMouseScrollCallback);
        glfwSwapInterval(1);
    }

    void Window::Destroy()
    {
        glfwDestroyWindow(s_WindowPtr);
        GraphicsContext::Destroy();
    }

    void Window::Update()
    {
        GraphicsContext::s_Context->SwapBuffers();
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

    void GLAPIENTRY Window::_MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                             GLchar const* message, void const* user_param)
    {
        auto const src_str = [source]() {
            switch (source)
            {
                case GL_DEBUG_SOURCE_API:
                    return "API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                    return "WINDOW SYSTEM";
                case GL_DEBUG_SOURCE_SHADER_COMPILER:
                    return "SHADER COMPILER";
                case GL_DEBUG_SOURCE_THIRD_PARTY:
                    return "THIRD PARTY";
                case GL_DEBUG_SOURCE_APPLICATION:
                    return "APPLICATION";
                case GL_DEBUG_SOURCE_OTHER:
                    return "OTHER";
            }
            return "";
        }();

        auto const type_str = [type]() {
            switch (type)
            {
                case GL_DEBUG_TYPE_ERROR:
                    return "ERROR";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    return "DEPRECATED_BEHAVIOR";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    return "UNDEFINED_BEHAVIOR";
                case GL_DEBUG_TYPE_PORTABILITY:
                    return "PORTABILITY";
                case GL_DEBUG_TYPE_PERFORMANCE:
                    return "PERFORMANCE";
                case GL_DEBUG_TYPE_MARKER:
                    return "MARKER";
                case GL_DEBUG_TYPE_OTHER:
                    return "OTHER";
            }
            return "";
        }();

        auto const severity_str = [severity]() {
            switch (severity)
            {
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    return "NOTIFICATION";
                case GL_DEBUG_SEVERITY_LOW:
                    return "LOW";
                case GL_DEBUG_SEVERITY_MEDIUM:
                    return "MEDIUM";
                case GL_DEBUG_SEVERITY_HIGH:
                    return "HIGH";
            }
            return "";
        }();
        LOG("%s, %s, %s, %u: %s\n", src_str, type_str, severity_str, id, message);
    }

    void Window::_ErrorCallback(int code, const char* err_str)
    {
        if (std::string(err_str) != "Invalid window attribute 0x0002000D")
            LOG_ERROR("%i GLFW Error: %s", code, err_str);
    }

}// namespace Engine
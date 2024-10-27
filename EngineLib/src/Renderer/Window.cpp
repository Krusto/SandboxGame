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
        constexpr auto OPENGL_VERSION_MINOR = 1;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
            glDebugMessageCallback(_MessageCallback, 0);
        }
        glfwSetWindowCloseCallback(this->s_WindowPtr, _CloseCallback);
        glfwSetWindowSizeCallback(this->s_WindowPtr, _WindowSizeCallback);
        glfwSetKeyCallback(this->s_WindowPtr, _WindowKeyCallback);
        glfwSetCursorPosCallback(this->s_WindowPtr, _WindowMouseMoveCallback);
    }

    void Window::Destroy() { glfwDestroyWindow(s_WindowPtr); }

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

    void GLAPIENTRY Window::_MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                             const GLchar* message, const void* userParam)
    {
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
    }

    void Window::_ErrorCallback(int code, const char* err_str)
    {
        if (std::string(err_str) != "Invalid window attribute 0x0002000D")
            LOG_ERROR("%i GLFW Error: %s", code, err_str);
    }

}// namespace Engine
#include "Window.hpp"

#include "GraphicsContext.hpp"
#include "Renderer.hpp"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

void ErrorCallback(int code, const char* err_str)
{
    if (std::string(err_str) != "Invalid window attribute 0x0002000D")
        LOG_ERROR(std::to_string(code) + " GLFW Error: " + std::string(err_str));
}

namespace Engine
{
    Window::Window(WindowSpec& spec)
    {
        this->spec = spec;

        if (!glfwInit())
        {
            LOG_ERROR("GLFW CAN NOT INITIALIZE!!!");
            exit(-1);
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

        int major, minor, rev;
        glfwGetVersion(&major, &minor, &rev);

        LOG_INFO("GLFW VERSION: " + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(rev));

        s_Window = glfwCreateWindow(spec.width, spec.height, spec.title.c_str(), nullptr, nullptr);
        if (this->Good())
        {
            LOG_ERROR("Failed to create GLFW window");
            glfwTerminate();
            exit(-1);
        }

        glfwMakeContextCurrent(s_Window);

        GraphicsContext::s_Context = GraphicsContext::Create(s_Window);
        GraphicsContext::s_Context->Init();

        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
        glfwSetErrorCallback(ErrorCallback);
        glfwSetWindowCloseCallback(this->s_Window, closeCallback);
        glfwSetWindowSizeCallback(this->s_Window, windowSizeCallback);
        glfwSetKeyCallback(this->s_Window, windowKeyCallback);
        glfwSetCursorPosCallback(this->s_Window, windowMouseMoveCallback);
        //    glfwSetInputMode(this->s_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Window::Update()
    {
        GraphicsContext::s_Context->SwapBuffers();
        this->spec.width = s_ViewportSize.width;
        this->spec.height = s_ViewportSize.height;
    }

    void Window::Clear(float r, float g, float b, float a) { Renderer::ClearColor(glm::vec4{r, g, b, a}); }

    double Window::GetDeltaTime() const { return 1.0 / m_Timestep; }

    void Window::SetDeltaTime(double value) { m_Timestep = value; }

    bool Window::Good() { return (s_Window == nullptr); }

    void Window::Close() { glfwSetWindowShouldClose(s_Window, GLFW_TRUE); }

    Window::operator bool() { return false; }
}// namespace Engine
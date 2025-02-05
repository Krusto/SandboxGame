#include <Core/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Renderer/Predefines.hpp>

EXPORT_RENDERER void RendererContextInit(void* handle) {}

EXPORT_RENDERER void RendererContextDestroy(void* handle) {}

EXPORT_RENDERER void RendererContextCreate(void* handle, void* window)
{
    glfwMakeContextCurrent((GLFWwindow*) window);

    if (!gladLoadGL())
    {
        LOG_ERROR("Failed to initalize OpenGL context\n");
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    // Enables Cull Facing
    // glEnable(GL_CULL_FACE);

    // Keeps front faces
    // glCullFace(GL_BACK);
    // Uses counter clock-wise standard
    glFrontFace(GL_CCW);

    const char* version = (const char*) glGetString(GL_VERSION);
    LOG("OPENGL VERSION %s!\n", version);
}

EXPORT_RENDERER void RendererContextSwapBuffers(void* window)
{
    glfwSwapBuffers((GLFWwindow*) window);
    glfwPollEvents();
}

EXPORT_RENDERER void _MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
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

EXPORT_RENDERER void RendererContextAddDebugMessanger(void* window)
{
    if (glfwGetWindowAttrib((GLFWwindow*) window, GLFW_CONTEXT_VERSION_MAJOR) == 4 &&
        glfwGetWindowAttrib((GLFWwindow*) window, GLFW_CONTEXT_VERSION_MINOR) >= 3)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(_MessageCallback, 0);
    }
}

EXPORT_RENDERER void RendererContextSetupWindowHints(void* window)
{
    constexpr auto OPENGL_VERSION_MAJOR = 4;
    constexpr auto OPENGL_VERSION_MINOR = 6;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
}
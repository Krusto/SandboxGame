#pragma once
#include <Renderer/GraphicsContext.hpp>
struct GLFWwindow;

class OpenGLContext: public GraphicsContext
{
public:
    OpenGLContext() = default;

    OpenGLContext(GLFWwindow* handle) : m_WindowHandle(handle) {}

    ~OpenGLContext() = default;

    virtual void Init() override;
    virtual void SwapBuffers() override;

private:
    GLFWwindow* m_WindowHandle{};
};
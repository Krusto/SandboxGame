#pragma once
#include <Renderer/GraphicsContext.hpp>
struct GLFWwindow;

class OpenGLContext: public GraphicsContext
{
public:
    OpenGLContext() = default;

    OpenGLContext(GLFWwindow* handle) : GraphicsContext(handle) {}

    ~OpenGLContext() = default;

public:
    virtual void Init() override;
    virtual void SwapBuffers() override;
};
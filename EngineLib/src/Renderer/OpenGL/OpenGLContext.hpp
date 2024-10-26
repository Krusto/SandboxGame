#pragma once
#include <Renderer/GraphicsContext.hpp>
struct GLFWwindow;

class OpenGLContext: public GraphicsContext
{
public:
    OpenGLContext() = default;

    OpenGLContext(GLFWwindow* handle) : GraphicsContext(handle) {}

    ~OpenGLContext(){LOG_ERROR("DESTROYING OPENGL CONTEXT\n");}

public:
    virtual void Init() override;
    virtual void SwapBuffers() override;
};
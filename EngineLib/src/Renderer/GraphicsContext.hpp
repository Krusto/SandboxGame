#pragma once
#include <Core/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLFW/glfw3native.h"

class GraphicsContext
{
public:
    GraphicsContext(GLFWwindow* handle) : p_WindowPtr(handle) {}

public:
    static GraphicsContext* Create(GLFWwindow* window);

public:
    virtual ~GraphicsContext(){
        LOG_ERROR("DESTROYING GRAPHICS CONTEXT");
    }

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;

public:
    static GraphicsContext* s_Context;

protected:
    GLFWwindow* p_WindowPtr;
};

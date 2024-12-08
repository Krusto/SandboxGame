#pragma once
#include <Core/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class GraphicsContext
{
public:
    GraphicsContext() = default;

    GraphicsContext(GLFWwindow* handle) : p_WindowPtr(handle) {}

    ~GraphicsContext() = default;

public:
    static GraphicsContext* Create(GLFWwindow* window);
    static void Destroy();

public:
    void Init();
    void SwapBuffers();

public:
    static GraphicsContext* s_Context;

protected:
    GLFWwindow* p_WindowPtr{};
};

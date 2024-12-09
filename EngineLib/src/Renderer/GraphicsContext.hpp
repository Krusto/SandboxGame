#pragma once
#include <Core/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <Renderer/Predefines.hpp>

class EXPORT_RENDERER GraphicsContext
{
public:
    GraphicsContext() = default;

    GraphicsContext(GLFWwindow* handle) : p_WindowPtr(handle) {}

    ~GraphicsContext() = default;

public:
    static GraphicsContext* Get();

public:
    void Create(GLFWwindow* window);
    void Destroy();
    void Init();
    void SwapBuffers();

protected:
    GLFWwindow* p_WindowPtr{};
};

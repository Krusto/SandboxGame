#pragma once

#include <Core/Log.hpp>

typedef struct GLFWwindow GLFWwindow;

class GraphicsContext
{
public:
    GraphicsContext() = default;

    GraphicsContext(GLFWwindow* handle) : p_WindowPtr(handle) {}

    ~GraphicsContext() = default;

public:
    static GraphicsContext* Get();
    static void SetupWindowHints();

public:
    void Create(GLFWwindow* window);
    void Destroy();
    void Init();
    void SwapBuffers();
    void AddDebugMessanger();

protected:
    GLFWwindow* p_WindowPtr{};
};

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
    static void Create(GLFWwindow* window);

public:
    void Destroy();
    void Init();
    void SwapBuffers();
    void AddDebugMessanger();

protected:
    GLFWwindow* p_WindowPtr{};
};

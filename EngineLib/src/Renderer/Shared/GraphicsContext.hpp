﻿#pragma once

#include <Core/Log.hpp>
#include <Renderer/Predefines.hpp>

typedef struct GLFWwindow GLFWwindow;

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
    void AddDebugMessanger();
    void SetupWindowHints();

protected:
    GLFWwindow* p_WindowPtr{};
};

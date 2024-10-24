#include <Core/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLContext.hpp"

#include <iostream>

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);

    if (!gladLoadGL())
    {
        LOG_ERROR("Failed to initalize OpenGL context");
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_WindowHandle);
    glfwPollEvents();
}

#include <Core/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGLContext.hpp"

#include <iostream>

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(p_WindowPtr);

    if (!gladLoadGL())
    {
        LOG_ERROR("Failed to initalize OpenGL context\n");
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(p_WindowPtr);
    glfwPollEvents();
}

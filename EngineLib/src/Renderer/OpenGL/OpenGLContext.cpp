#include <Core/Log.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Renderer/GraphicsContext.hpp>

void GraphicsContext::Init()
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

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    // Enables Cull Facing
    // glEnable(GL_CULL_FACE);

    // Keeps front faces
    // glCullFace(GL_BACK);
    // Uses counter clock-wise standard
    glFrontFace(GL_CCW);
}

void GraphicsContext::SwapBuffers()
{
    glfwSwapBuffers(p_WindowPtr);
    glfwPollEvents();
}

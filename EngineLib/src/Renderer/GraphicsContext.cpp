#include "GraphicsContext.hpp"
#include <Renderer/OpenGL/OpenGLContext.hpp>
GraphicsContext* GraphicsContext::s_Context;

GraphicsContext* GraphicsContext::Create(GLFWwindow* window) { return new OpenGLContext(window); }

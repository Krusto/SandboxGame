#include "VertexArray.hpp"
#include <Renderer/OpenGL/OpenGLVertexArray.hpp>
#include <Renderer/RendererAPI.hpp>
#include <Core/Allocator.hpp>

namespace Engine
{

    VertexArray* VertexArray::Create(uint32_t indexCount)
    {
        VertexArray* ptr = Engine::Allocator::Allocate<OpenGLVertexArray>();
        ptr->Init(indexCount);
        return ptr;
    }

}// namespace Engine
#pragma once
#include <cstdint>
#include <Renderer/VertexArray.hpp>

namespace Engine
{

    class OpenGLVertexArray: public VertexArray
    {
    public:
        OpenGLVertexArray() = default;
        ~OpenGLVertexArray();

        explicit OpenGLVertexArray(uint32_t indexCount);

        void Bind() const override;

        void Unbind() const override;
    };
}// namespace Engine
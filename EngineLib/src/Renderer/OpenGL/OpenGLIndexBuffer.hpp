#pragma once

#include <Renderer/VertexArray.hpp>
#include <Renderer/IndexBuffer.hpp>
#include <cstdint>

namespace Engine
{
    class OpenGLIndexBuffer: public IndexBuffer
    {
    public:
        OpenGLIndexBuffer() = default;

        OpenGLIndexBuffer(const uint32_t* data, uint32_t length);
        ~OpenGLIndexBuffer() = default;

        static OpenGLIndexBuffer Create(const uint32_t* data, uint32_t length);

        void Bind() const override;

    private:
    };
}// namespace Engine
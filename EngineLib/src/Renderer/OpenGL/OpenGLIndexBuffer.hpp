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
        ~OpenGLIndexBuffer() = default;

        void Bind() const override;

        void Destroy() override;

        void Init(VertexArray* va, const uint32_t* data, uint32_t length) override;

    private:
    };
}// namespace Engine
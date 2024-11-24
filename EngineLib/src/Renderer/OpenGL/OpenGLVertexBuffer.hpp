#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <Renderer/VertexBuffer.hpp>
#include <Renderer/VertexLayout.hpp>
#include <Renderer/Vertex.hpp>

namespace Engine
{

    class OpenGLVertexBuffer: public VertexBuffer
    {
    public:
        OpenGLVertexBuffer() = default;
        ~OpenGLVertexBuffer() = default;

    public:
        void Init(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length) override;

        void Bind() const override;

        size_t GetSize() const override;

        void Destroy() override;

    };
}// namespace Engine
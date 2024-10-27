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

        OpenGLVertexBuffer(const VertexLayout& layout, float* data, uint32_t length);

    public:
        void Bind() const;

        uint32_t GetID() const;

        size_t GetSize() const;

    private:
        uint32_t m_ID{};
    };
}// namespace Engine
#pragma once
#include <cstdint>
#include <Renderer/VertexArray.hpp>

namespace Engine
{

    class OpenGLVertexArray: public VertexArray
    {
    public:
        OpenGLVertexArray() = default;
        ~OpenGLVertexArray() = default;

        void Init(uint32_t indexCount) override;

        void Bind() const override;

        void Unbind() const override;

        void Destroy() override;

        void AddVertexBuffer(const VertexLayout& layout, float* data, uint32_t length) override;
        void AddIndexBuffer(const uint32_t* data, uint32_t length) override;
    };
}// namespace Engine
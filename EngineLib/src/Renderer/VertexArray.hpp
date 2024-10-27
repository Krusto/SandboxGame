#pragma once
#include <cstdint>
#include <Core/Ref.hpp>
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace Engine
{

    class VertexArray
    {
    public:
        static VertexArray* Create(uint32_t indexCount);

    public:
        virtual ~VertexArray() = default;

        virtual void Init(uint32_t indexCount) = 0;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void Destroy() = 0;

    public:
        uint32_t id() { return m_ID; }

        void AddVertexBuffer(VertexBuffer* vertexBuffer);
        void AddIndexBuffer(IndexBuffer* indexBuffer);

    public:
        uint32_t IndexCount{};

    protected:
        VertexBuffer* m_VertexBuffer;
        IndexBuffer* m_IndexBuffer;
        uint32_t m_ID{};
    };
}// namespace Engine
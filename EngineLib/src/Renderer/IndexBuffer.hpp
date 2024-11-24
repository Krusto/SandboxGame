#pragma once
#include <cstdint>

namespace Engine
{
    class VertexArray;
    class IndexBuffer
    {
    public:
        static IndexBuffer* Create(VertexArray* va, const uint32_t* data, uint32_t length);

    public:
        virtual ~IndexBuffer() = default;

        virtual void Init(VertexArray* va, const uint32_t* data, uint32_t length) = 0;

        virtual void Destroy() = 0;

        virtual void Bind() const = 0;

        virtual uint32_t GetID() { return m_ID; }

    public:
        uint32_t indexCount{};

    protected:
        uint32_t m_ID;
    };
}// namespace Engine
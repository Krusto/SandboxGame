#pragma once
#include <cstdint>
#include <Core/Ref.hpp>
#include <Renderer/Vertex.hpp>

namespace Engine
{
    class VertexBuffer
    {
    public:
        static VertexBuffer* Create(const VertexLayout& layout, float* data, uint32_t length);

    public:
        virtual ~VertexBuffer(){};

        virtual void Init(const VertexLayout& layout, float* data, uint32_t length) = 0;

        virtual void Bind() const = 0;

        virtual size_t GetSize() const = 0;

        virtual void Destroy() = 0;

    public:
        uint32_t GetID() const { return m_ID; }

    protected:
        uint32_t m_ID;
    };
}// namespace Engine
#pragma once
#include <cstdint>

namespace Engine
{
    class VertexArray;
    struct VertexLayout;
    struct VertexBufferData;

    class VertexBuffer
    {
    public:
        VertexBuffer() = default;
        ~VertexBuffer() = default;
        static VertexBuffer Create(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length);

    public:
        void Init(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length);
        void Bind() const;
        size_t GetSize() const;
        void Destroy();
        uint32_t GetID() const;

    private:
        VertexBufferData* m_Data{};
    };
}// namespace Engine
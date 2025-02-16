#pragma once
#include <cstdint>

namespace Engine
{
    class VertexArray;
    struct IndexBufferData;

    class IndexBuffer
    {
    public:
        IndexBuffer() = default;
        IndexBuffer(VertexArray* va, const uint32_t* data, uint32_t length);
        ~IndexBuffer() = default;

    public:
        static IndexBuffer Create(VertexArray* va, const uint32_t* data, uint32_t length);

    public:
        void Init(VertexArray* va, const uint32_t* data, uint32_t length);
        void Destroy();
        void Bind() const;
        uint32_t GetID() const;
        uint32_t GetSize() const;
        uint32_t GetLength() const;

    private:
        IndexBufferData* m_Data{};
    };
}// namespace Engine

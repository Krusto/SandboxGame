#pragma once
#include <cstdint>
#include <Core/Ref.hpp>
#include <Renderer/Vertex.hpp>
#include <Renderer/Predefines.hpp>

namespace Engine
{
    class  VertexArray;
    struct VertexBufferData;

    class  EXPORT_RENDERER VertexBuffer
    {
    public:
        static VertexBuffer Create(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length);

    public:
        void Init(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length);
        void Bind() const;
        size_t GetSize() const;
        void Destroy();
        uint32_t GetID() const;

    private:
        VertexBufferData* m_Data;
    };
}// namespace Engine
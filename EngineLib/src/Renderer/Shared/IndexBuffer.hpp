#pragma once
#include <cstdint>
#include <Renderer/Predefines.hpp>

namespace Engine
{
    class VertexArray;
    struct IndexBufferData;

    class EXPORT_RENDERER IndexBuffer
    {
    public:
        static IndexBuffer Create(VertexArray* va, const uint32_t* data, uint32_t length);

    public:
        void Init(VertexArray* va, const uint32_t* data, uint32_t length);
        void Destroy();
        void Bind() const;
        uint32_t GetID() const;

    public:
        uint32_t indexCount{};

    private:
        IndexBufferData* m_Data{};
    };
}// namespace Engine

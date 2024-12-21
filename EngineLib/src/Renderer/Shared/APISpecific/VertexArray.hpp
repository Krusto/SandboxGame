#pragma once
#include <cstdint>
#include <Renderer/Predefines.hpp>
#include <Renderer/Shared/VertexLayout.hpp>

namespace Engine
{
    struct VertexArrayData;

    class EXPORT_RENDERER VertexArray
    {
    public:
        static VertexArray Create(uint32_t indexCount);

        VertexArray() = default;
        ~VertexArray() = default;

        void Init(uint32_t indexCount);
        void Bind() const;
        void Unbind() const;
        void Destroy();
        void AddVertexBuffer(const VertexLayout& layout, float* data, uint32_t length);
        void AddIndexBuffer(const uint32_t* data, uint32_t length);
        uint32_t IndexCount() const;
        uint32_t id() const;

        bool IsValid() const { return m_Data != nullptr; }

    private:
        VertexArrayData* m_Data{};
    };
}// namespace Engine

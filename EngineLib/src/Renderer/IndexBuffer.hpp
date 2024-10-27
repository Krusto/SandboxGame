#pragma once
#include <cstdint>

namespace Engine
{
    class IndexBuffer
    {
    public:
        static IndexBuffer* Create(const uint32_t* data, uint32_t length);

    public:
        virtual ~IndexBuffer() = default;
        virtual void Bind() const = 0;

        virtual uint32_t GetID() { return m_ID; }

    public:
        uint32_t indexCount{};

    protected:
        uint32_t m_ID;
    };
}// namespace Engine
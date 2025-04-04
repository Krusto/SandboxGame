#pragma once
#include <cstdint>
#include <Renderer/Shared/StorageBufferType.hpp>

namespace Engine
{
    using size_t = std::size_t;
    struct DrawIndirectBufferData;

    class DrawIndirectBuffer
    {
    public:
        DrawIndirectBuffer() = default;
        DrawIndirectBuffer(uint8_t* data, size_t size, StorageBufferType type);
        ~DrawIndirectBuffer() = default;

    public:
        static DrawIndirectBuffer Create(uint8_t* data, size_t size, StorageBufferType type);

    public:
        void Init(uint8_t* data, size_t size, StorageBufferType type);
        void Bind() const;
        void Upload(uint8_t* data, size_t size, size_t offset = 0) const;
        void Draw(size_t count, size_t stride, size_t offset = 0) const;
        void Unbind() const;
        void Destroy();
        uint32_t id() const;

    private:
        DrawIndirectBufferData* m_Data{};
    };
}// namespace Engine
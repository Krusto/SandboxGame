#pragma once
#include <Core/Types.hpp>
#include <Renderer/Shared/StorageBufferType.hpp>

namespace Engine
{
    struct DrawIndirectBufferData;

    class DrawIndirectBuffer
    {
    public:
        DrawIndirectBuffer() = default;
        DrawIndirectBuffer(uint8_t* data, std::size_t size, StorageBufferType type);
        ~DrawIndirectBuffer() = default;

    public:
        static DrawIndirectBuffer Create(uint8_t* data, std::size_t size, StorageBufferType type);

    public:
        void Init(uint8_t* data, std::size_t size, StorageBufferType type);
        void Bind() const;
        void Upload(uint8_t* data, std::size_t size, std::size_t offset = 0) const;
        void Draw(size_t count, std::size_t stride, std::size_t offset = 0) const;
        void Unbind() const;
        void Destroy();
        uint32_t id() const;

    private:
        DrawIndirectBufferData* m_Data{};
    };
}// namespace Engine
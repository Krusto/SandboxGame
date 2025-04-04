#pragma once
#include <cstdint>
#include <Renderer/Shared/StorageBufferType.hpp>

namespace Engine
{
    struct StorageBufferData;

    class StorageBuffer
    {
    public:
        StorageBuffer() = default;
        StorageBuffer(uint8_t* data, size_t size, StorageBufferType type);
        ~StorageBuffer() = default;

    public:
        static StorageBuffer Create(uint8_t* data, size_t size, StorageBufferType type);

    public:
        void Init(uint8_t* data, size_t size, StorageBufferType type);
        void Bind(size_t location) const;
        void Upload(uint8_t* data, size_t size, size_t offset = 0) const;
        void Unbind() const;
        void Destroy();
        uint32_t id() const;

    private:
        StorageBufferData* m_Data{};
    };
}// namespace Engine
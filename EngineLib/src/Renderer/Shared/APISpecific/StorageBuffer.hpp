#pragma once
#include <Core/Types.hpp>
#include <Renderer/Shared/StorageBufferType.hpp>

namespace Engine
{
    struct StorageBufferData;

    class StorageBuffer
    {
    public:
        StorageBuffer() = default;
        StorageBuffer(uint8_t* data, std::size_t size, StorageBufferType type);
        ~StorageBuffer() = default;

    public:
        static StorageBuffer Create(uint8_t* data, std::size_t size, StorageBufferType type);

    public:
        void Init(uint8_t* data, std::size_t size, StorageBufferType type);
        void Bind(std::size_t location) const;
        void Upload(uint8_t* data, std::size_t size, std::size_t offset = 0) const;
        void Unbind() const;
        void Destroy();
        uint32_t id() const;

    private:
        StorageBufferData* m_Data{};
    };
}// namespace Engine
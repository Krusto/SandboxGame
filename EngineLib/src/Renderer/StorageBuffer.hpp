#pragma once
#include <cstdint>

namespace Engine
{
    enum class StorageBufferType
    {
        None,
        DynamicStorage,
        MapReadOnly,
        MapWriteOnly,
        MapPersistent,
        MapCoherent,
        ClientStorage
    };

    class StorageBuffer
    {
    public:
        static StorageBuffer* Create(uint8_t* data, size_t size, StorageBufferType type);

    public:
        virtual ~StorageBuffer() = default;

    public:
        virtual void Init(uint8_t* data, size_t size, StorageBufferType type) = 0;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void Destroy() = 0;

    public:
        uint32_t id() { return m_ID; }

    protected:
        uint32_t m_ID{};
    };
}// namespace Engine
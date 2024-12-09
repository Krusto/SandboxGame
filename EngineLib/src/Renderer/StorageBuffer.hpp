#pragma once
#include <cstdint>
#include <Renderer/Predefines.hpp>

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

    class EXPORT_RENDERER StorageBuffer
    {
    public:
        static StorageBuffer* Create(uint8_t* data, size_t size, StorageBufferType type);

    public:
        virtual ~StorageBuffer() = default;

    public:
        virtual void Init(uint8_t* data, size_t size, StorageBufferType type) = 0;

        virtual void Bind(size_t location) const = 0;

        virtual void Unbind() const = 0;

        virtual void Destroy() = 0;

    public:
        uint32_t id() const { return m_ID; }

    protected:
        uint32_t m_ID{};
    };
}// namespace Engine
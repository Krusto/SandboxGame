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
    struct StorageBufferData;

    class EXPORT_RENDERER StorageBuffer
    {
    public:
        StorageBuffer() = default;
        ~StorageBuffer() = default;

    public:
        static StorageBuffer Create(uint8_t* data, size_t size, StorageBufferType type);

    public:
        void Init(uint8_t* data, size_t size, StorageBufferType type);
        void Bind(size_t location) const;
        void Unbind() const;
        void Destroy();
        uint32_t id() const;

    private:
        StorageBufferData* m_Data{};
    };
}// namespace Engine
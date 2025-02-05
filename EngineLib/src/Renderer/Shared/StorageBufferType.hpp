#pragma once

namespace Engine{
    enum class StorageBufferType : unsigned int
    {
        None = 0,
        DynamicStorage,
        MapReadOnly,
        MapWriteOnly,
        MapPersistent,
        MapCoherent,
        ClientStorage
    };
}
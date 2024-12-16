#pragma once
#include <cstdint>
#include <Renderer/StorageBuffer.hpp>

namespace Engine
{
    struct StorageBufferData {
        uint32_t id;
        StorageBufferType type;
    };
}// namespace Engine
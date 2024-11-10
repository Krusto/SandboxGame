#pragma once
#include <cstdint>
#include <Renderer/StorageBuffer.hpp>

namespace Engine
{

    class OpenGLStorageBuffer: public StorageBuffer
    {
    public:
        OpenGLStorageBuffer() = default;
        ~OpenGLStorageBuffer() = default;

    public:
        void Init(uint8_t* data, size_t size, StorageBufferType type) override;

        void Bind() const override;

        void Unbind() const override;

        void Destroy() override;
    };
}// namespace Engine
#pragma once
#include <cstdint>
#include <Renderer/VertexArray.hpp>
#include <Renderer/VertexBuffer.hpp>
#include <Renderer/IndexBuffer.hpp>

namespace Engine
{
    struct VertexArrayData {
        uint32_t indexCount{};
        VertexBuffer vertexBuffer{};
        IndexBuffer indexBuffer{};
        uint32_t id{};
    };

}// namespace Engine
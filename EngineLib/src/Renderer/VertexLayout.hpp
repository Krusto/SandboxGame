//
// Created by Stoyanov, Krusto (K.S.) on 6/23/2022.
//
#pragma once

#include <vector>
#include <string_view>

#include <Core/Core.hpp>
#include "ShaderDataType.hpp"

namespace Engine
{
    struct VertexAttribute {
    public:
        std::string_view name{};
        ShaderUniformType type;
        uint32_t offset{};
    };

    class EXPORT_RENDERER VertexLayout
    {
    public:
        VertexLayout() = default;
        VertexLayout(const VertexLayout&) = default;
        ~VertexLayout() = default;

        VertexLayout(std::initializer_list<VertexAttribute> attributes);

        std::vector<VertexAttribute> attributes;
        uint32_t stride{};

        VertexLayout& operator=(const VertexLayout& other) = default;
    };
}// namespace Engine
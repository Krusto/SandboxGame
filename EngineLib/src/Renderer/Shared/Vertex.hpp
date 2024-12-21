//
// Created by Stoyanov, Krusto (K.S.) on 6/23/2022.
//
#pragma once
#include <glm/glm.hpp>
#include "VertexLayout.hpp"

namespace Engine
{
    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoord;

        static VertexLayout GetLayout()
        {
            return {{"Position", ShaderUniformType::Vec3},
                    {"Normal", ShaderUniformType::Vec3},
                    {"TextureCoord", ShaderUniformType::Vec2}};
        }
    };
}// namespace Engine
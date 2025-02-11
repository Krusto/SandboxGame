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
            return CreateVertexLayout({{
                                        {"Position", (uint8_t) ShaderUniformType::Vec3, 0},
                                             {"Normal", (uint8_t)ShaderUniformType::Vec3,0},
                                             {"TextureCoord", (uint8_t)ShaderUniformType::Vec2,0}
                                            },3});
        }
    };
}// namespace Engine
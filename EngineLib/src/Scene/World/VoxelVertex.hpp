#pragma once
#include <glm/glm.hpp>
#include <Renderer/VertexLayout.hpp>

struct VoxelVertex {
    glm::vec3 pos;
    int32_t axis;
    glm::vec2 texCoord;

    static Engine::VertexLayout GetLayout()
    {
        return {{"Position", Engine::ShaderUniformType::Vec3},
                {"Axis", Engine::ShaderUniformType::Int},
                {"TextureCoord", Engine::ShaderUniformType::Vec2}};
    }
};
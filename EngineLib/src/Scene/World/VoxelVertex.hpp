#pragma once
#include <glm/glm.hpp>
#include <Renderer/VertexLayout.hpp>

namespace Engine
{

    struct VoxelVertex {
        glm::vec3 pos;
        uint32_t compressedData;

        static Engine::VertexLayout GetLayout()
        {
            return {{"Position", Engine::ShaderUniformType::Vec3},
                    {"CompressedData", Engine::ShaderUniformType::UInt}};
        }
    };
}// namespace Engine
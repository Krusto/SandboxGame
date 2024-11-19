#pragma once
#include <glm/glm.hpp>
#include <Renderer/VertexLayout.hpp>

namespace Engine
{

    struct VoxelVertex {
        uint32_t compressedData;

        static Engine::VertexLayout GetLayout()
        {
            return {{"CompressedData", Engine::ShaderUniformType::UInt}};
        }
    };
}// namespace Engine
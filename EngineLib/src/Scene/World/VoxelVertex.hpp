#pragma once
#include <glm/glm.hpp>
#include <Renderer/Shared/VertexLayout.hpp>

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
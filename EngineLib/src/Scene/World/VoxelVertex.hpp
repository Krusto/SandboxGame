#pragma once
#include <glm/glm.hpp>
#include <Renderer/Shared/VertexLayout.hpp>

namespace Engine
{

    struct VoxelVertex {
        uint32_t compressedData;

        static Engine::VertexLayout GetLayout()
        {
            return CreateVertexLayout(VertexAttributeList{
                {
                    VertexAttribute{"CompressedData", (uint8_t)Engine::ShaderUniformType::UInt}
                },
                1
            });
        }
    };
}// namespace Engine
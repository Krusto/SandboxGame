#pragma once
#include <Renderer/CubemapTexture.hpp>
#include <Renderer/Shader.hpp>
#include <Renderer/VertexArray.hpp>

namespace Engine
{
    struct SkyboxData {
        CubemapTexture* m_Cubemap{};
        VertexArray* m_VertexArray{};
        Shader* m_Shader{};
        std::string m_CubemapName{};
        const float m_RotationSpeed{0.001f};
        glm::vec3 m_Rotation{};
    };
}// namespace Engine
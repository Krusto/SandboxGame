#pragma once
#include <Renderer/CubemapTexture.hpp>
#include <Renderer/Shader.hpp>
#include <Renderer/VertexArray.hpp>
#include <Renderer/RendererCommand.hpp>
#include <Renderer/Camera.hpp>

namespace Engine
{
    class Skybox
    {
    public:
        Skybox() = default;
        ~Skybox() = default;

    public:
        static Skybox* Create(std::string_view cubemapName, const std::string& shaderPath,
                              const std::unordered_map<CubemapTextureFace, std::string>& Path);

    public:
        void Load(std::string_view cubemapName, const std::string& shaderPath,
                  const std::unordered_map<CubemapTextureFace, std::string>& Path);

        void Destroy();

        void Reload();

        void Draw(Camera* camera) const;

        void Update(float dt);

        RendererCommand BindTexture(uint32_t slot = 0) const;
        RendererCommand RenderCommand(Camera* camera) const;

    private:
        CubemapTexture* m_Cubemap{};
        VertexArray* m_VertexArray{};
        Shader* m_Shader{};
        std::string m_CubemapName{};
        const float m_RotationSpeed{0.001f};
        glm::vec3 m_Rotation{};
    };
}// namespace Engine
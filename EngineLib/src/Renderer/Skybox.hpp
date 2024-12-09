#pragma once
#include <Renderer/CubemapTexture.hpp>
#include <Renderer/RendererCommand.hpp>
#include <Renderer/Camera.hpp>
#include <Renderer/Predefines.hpp>

namespace Engine
{
    struct SkyboxData;

    class EXPORT_RENDERER Skybox
    {
    public:
        static Skybox Create(std::string_view cubemapName, const std::string& shaderPath,
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
        SkyboxData* m_Data{};
    };
}// namespace Engine
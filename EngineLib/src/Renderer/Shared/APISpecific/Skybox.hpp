#pragma once
#include <Renderer/Shared/APISpecific/CubemapTexture.hpp>
#include <Renderer/Shared/RendererCommand.hpp>
#include <Renderer/Shared/Camera.hpp>
#include <Renderer/Predefines.hpp>
#include <Core/STL/Containers.hpp>

namespace Engine
{
    struct SkyboxData;

    class Skybox
    {
    public:
        Skybox() = default;
        Skybox(std::string_view cubemapName, const std::string& shaderPath,
               const Vector<Pair<CubemapTextureFace, const char*>>* Path);
        ~Skybox() = default;

    public:
        static Skybox Create(std::string_view cubemapName, const std::string& shaderPath,
                             const Vector<Pair<CubemapTextureFace, const char*>>* Path);

    public:
        void Load(std::string_view cubemapName, const std::string& shaderPath,
                  const Vector<Pair<CubemapTextureFace, const char*>>* Path);

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
#pragma once
#include <string_view>
#include <Engine.hpp>

class SandboxSky
{
public:
    SandboxSky() = default;
    ~SandboxSky() = default;

    void Create(std::string_view shaderPath, std::string_view texturePath, std::string_view cubemapName);

    inline void Render(Engine::Camera* camera) const { Engine::Renderer::Submit(m_Skybox.RenderCommand(camera)); }

    inline void BindTexture(uint32_t slot = 0) const { Engine::Renderer::Submit(m_Skybox.BindTexture(slot)); }

    inline void Update(float dt) { m_Skybox.Update(dt); }

    inline void Destroy() { m_Skybox.Destroy(); }

    inline void Reload() { m_Skybox.Reload(); }

private:
    Engine::Skybox m_Skybox;
};
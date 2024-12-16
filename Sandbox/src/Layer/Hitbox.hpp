#pragma once
#include <glm/glm.hpp>
#include <Renderer/Renderer.hpp>

class Hitbox
{
public:
    Hitbox() = default;
    ~Hitbox();

public:
    void Init();
    void Destroy();

    Engine::RendererCommand Render(Engine::Shader* shader,float dt, uint32_t axis) const;

public:
    glm::vec3 position;

private:
private:
    Engine::VertexArray m_VertexArray;
};
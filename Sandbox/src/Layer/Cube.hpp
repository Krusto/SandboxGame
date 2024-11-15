#pragma once
#include <glm/glm.hpp>
#include <Renderer/Renderer.hpp>

class Cube
{
public:
    Cube() = default;
    ~Cube();

public:
    void Init();
    void Draw(uint32_t axis,Engine::Shader* shader,Engine::Camera* camera) const;
    void Destroy();

public:
    glm::vec3 position;

private:
    Engine::RendererCommand Render(uint32_t axis, Engine::Shader* shader, Engine::Camera* camera) const;

private:
    Engine::VertexArray* m_VertexArray;
};
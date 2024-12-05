#pragma once
#include <glm/glm.hpp>
#include <Renderer/Renderer.hpp>

class DebugCube
{
public:
    DebugCube() = default;
    ~DebugCube();

public:
    void Init();
    void Destroy();

    Engine::RendererCommand Render(Engine::Shader* shader) const;

public:
    glm::vec3 position;

private:
private:
    Engine::VertexArray* m_VertexArray;
};
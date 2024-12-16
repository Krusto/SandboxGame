#pragma once
#include <glm/glm.hpp>
#include <Renderer/Renderer.hpp>

class LightObject
{
public:
    LightObject() = default;
    ~LightObject();

public:
    void Init();
    void Destroy();

    Engine::RendererCommand Render(Engine::Shader* shader) const;
    Engine::RendererCommand UploadLight(Engine::Shader* shader) const;

public:
    glm::vec3 rotation;
    glm::vec3 position;
    glm::vec3 ambient{0.2f, 0.2f, 0.2f};
    glm::vec3 diffuse{1.0f, 1.0f, 1.0f};
    glm::vec3 specular{0.0f, 0.0f, 0.0f};
    float intensity{1.0f};

private:
private:
    Engine::VertexArray m_VertexArray;
};
#include "Cube.hpp"
#include <glm/gtx/transform.hpp>

Cube::~Cube() { Destroy(); }

void Cube::Init()
{

    m_VertexArray = Engine::VertexArray::Create(6);

    // VoxelVertex{glm::vec3(0, 0, 1)},
    // VoxelVertex{glm::vec3(0, 0, 0)},
    // VoxelVertex{glm::vec3(1, 0, 0)},
    // VoxelVertex{glm::vec3(1, 0, 1)}};

    float cubeVertices[] = {//   Coordinates
                            0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0};
    uint32_t cubeIndices[] = {// Right
                              0, 1, 2, 0, 2, 3};

    Engine::VertexLayout layout({{"Position", Engine::ShaderUniformType::Vec3}});

    m_VertexArray->Bind();
    m_VertexArray->AddVertexBuffer(Engine::VertexBuffer::Create(layout, (float*) cubeVertices, 4));
    m_VertexArray->AddIndexBuffer(Engine::IndexBuffer::Create(cubeIndices, 6));
}

void Cube::Draw(uint32_t axis, Engine::Shader* shader, Engine::Camera* camera) const
{
    Engine::Renderer::Submit(Render(axis, shader, camera));
}

void Cube::Destroy()
{
    if (m_VertexArray)
    {
        m_VertexArray->Destroy();
        Engine::Allocator::Deallocate(m_VertexArray);
        m_VertexArray = nullptr;
    }
}

Engine::RendererCommand Cube::Render(uint32_t axis, Engine::Shader* shader, Engine::Camera* camera) const
{
    return Engine::RendererCommand([=, a = axis]() {
        glDepthFunc(GL_LESS);
        //Engine::Renderer::SwitchWireframeMode();
        shader->Bind();
        m_VertexArray->Bind();
        camera->Upload(shader);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shader->SetUniform("model", model);
        shader->SetUniform("axis", a);
        shader->SetUniform("pos", position);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        Engine::Renderer::SwitchFillMode();
        glDepthFunc(GL_LESS);
    });
}
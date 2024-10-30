#include "World.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/RendererCommand.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine
{
    void World::Init(int seed)
    {
        m_Seed = seed;
        auto vertices = std::vector<Vertex>({Vertex{glm::vec3{-0.5, -0.5, 0}, glm::vec3{1, 1, 0}, glm::vec2{0, 0}},
                                             Vertex{glm::vec3{-0.5, 0.5, 0}, glm::vec3{0, 1, 0}, glm::vec2{0, 1}},
                                             Vertex{glm::vec3{0.5, 0.5, 0}, glm::vec3{0, 1, 1}, glm::vec2{1, 1}},
                                             Vertex{glm::vec3{0.5, -0.5, 0}, glm::vec3{1, 0, 1}, glm::vec2{1, 0}}});
        std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};

        m_VertexArray = VertexArray::Create((uint32_t) indices.size());
        m_VertexArray->Bind();

        m_VertexArray->AddVertexBuffer(
                VertexBuffer::Create(Vertex::GetLayout(), (float*) vertices.data(), (uint32_t) vertices.size()));
        m_VertexArray->AddIndexBuffer(IndexBuffer::Create(indices.data(), (uint32_t) indices.size()));


        m_VertexArray->Unbind();
    }

    void World::Destroy()
    {
        if (m_VertexArray)
        {
            m_VertexArray->Destroy();
            delete m_VertexArray;
            m_VertexArray = nullptr;
        }
    }

    void World::OnUpdate(float dt) { m_Time += dt * 1000; };

    void World::Draw(Shader* shader) const
    {
        if (shader)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_Position);
            model = glm::rotate(model, glm::radians(m_Time), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(m_Time), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(m_Time), glm::vec3(0.0f, 0.0f, 1.0f));
            Renderer::Submit(GetRenderCommand(shader, m_VertexArray, model));
        }
    }

    RendererCommand World::GetRenderCommand(const Shader* const shader, const VertexArray* const va,
                                            const glm::mat4& model) const
    {
        return RendererCommand([shader, va, model]() {
            shader->Bind();
            shader->SetUniform("model", model);
            va->Bind();
            glDrawElements(GL_TRIANGLES, va->IndexCount, GL_UNSIGNED_INT, nullptr);
            va->Unbind();
        });
    }

    void World::Generate() {}
}// namespace Engine
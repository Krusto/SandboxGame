#include "World.hpp"

namespace Engine
{
    void World::Init(int seed)
    {
        m_Seed = seed;
        auto vertices = std::vector<Vertex>({Vertex{glm::vec3{-0.5, -0.5, 0}, glm::vec3{0, 1, 0}, glm::vec2{0, 0}},
                                             Vertex{glm::vec3{-0.5, 0.5, 0}, glm::vec3{0, 1, 0}, glm::vec2{0, 1}},
                                             Vertex{glm::vec3{0.5, 0.5, 0}, glm::vec3{0, 1, 0}, glm::vec2{1, 1}},
                                             Vertex{glm::vec3{0.5, -0.5, 0}, glm::vec3{0, 1, 0}, glm::vec2{1, 0}}});
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

    void World::OnUpdate(float dt){};

    void World::Draw(Shader* shader) const
    {
        if (shader)
        {
            shader->Bind();
            m_VertexArray->Bind();
            glDrawElements(GL_TRIANGLES, m_VertexArray->IndexCount, GL_UNSIGNED_INT, nullptr);
            m_VertexArray->Unbind();
        }
    }

    void World::GenerateWorld() {}
}// namespace Engine
#include "Hitbox.hpp"
#include <glm/gtx/transform.hpp>

Hitbox::~Hitbox() { Destroy(); }

void Hitbox::Init()
{

    uint32_t indices[] = {0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
                          12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};

    m_VertexArray = Engine::VertexArray::Create(36);
    m_VertexArray->Bind();
    m_VertexArray->AddIndexBuffer(indices, 36);
}

void Hitbox::Destroy()
{
    if (m_VertexArray)
    {
        m_VertexArray->Destroy();
        Engine::Allocator::Deallocate(m_VertexArray);
        m_VertexArray = nullptr;
    }
}

Engine::RendererCommand Hitbox::Render(Engine::Shader* shader,float dt, uint32_t axis) const
{
    return Engine::RendererCommand([=]() {
        glDepthFunc(GL_ALWAYS);
        //glDepthFunc(GL_GEQUAL);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shader->Bind();
        shader->SetUniform("axis", axis);
        shader->SetUniform("delta", dt);
        m_VertexArray->Bind();
        // glm::mat4 model = glm::translate(glm::mat4(1.0f), position + glm::vec3(0.5, 0.5, 0.5));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
        shader->SetUniform("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDepthFunc(GL_LESS);
    });
}
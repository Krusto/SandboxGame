#include "DebugCube.hpp"
#include <glm/gtx/transform.hpp>

DebugCube::~DebugCube() { Destroy(); }

void DebugCube::Init()
{

    float vertices[] = {
            // Vertex positions (x, y, z)
            -1.0f, -1.0f, -1.0f, 0, 0,// 0: Back-bottom-left
            1.0f,  -1.0f, -1.0f, 1, 0,// 1: Back-bottom-right
            -1.0f, 1.0f,  -1.0f, 0, 1,// 2: Back-top-left
            1.0f,  1.0f,  -1.0f, 1, 1,// 3: Back-top-right
            -1.0f, -1.0f, 1.0f,  0, 0,// 4: Front-bottom-left
            1.0f,  -1.0f, 1.0f,  1, 0,// 5: Front-bottom-right
            -1.0f, 1.0f,  1.0f,  0, 1,// 6: Front-top-left
            1.0f,  1.0f,  1.0f,  1, 0 // 7: Front-top-right
    };
    unsigned int indices[] = {// Front face
                              4, 5, 6, 5, 7, 6,
                              // Back face
                              0, 2, 1, 1, 2, 3,
                              // Left face
                              0, 4, 2, 2, 4, 6,
                              // Right face
                              1, 3, 5, 3, 7, 5,
                              // Top face
                              2, 6, 3, 3, 6, 7,
                              // Bottom face
                              0, 1, 4, 1, 5, 4};

    Engine::VertexLayout layout = Engine::CreateVertexLayout(Engine::VertexAttributeList{
            {Engine::VertexAttribute{"Position", (uint8_t) Engine::ShaderUniformType::Vec3},
             Engine::VertexAttribute{"TextureCoord", (uint8_t) Engine::ShaderUniformType::Vec2}},
            2});

    m_VertexArray = Engine::VertexArray::Create(36);
    m_VertexArray.Bind();
    m_VertexArray.AddVertexBuffer(&layout, (float*) vertices, sizeof(vertices) / sizeof(float));
    m_VertexArray.AddIndexBuffer(indices, 36);
}

void DebugCube::Destroy() { m_VertexArray.Destroy(); }

Engine::RendererCommand DebugCube::Render(Engine::Shader* shader) const
{
    return Engine::RendererCommand([debugCube = this, shader]() {
        shader->Bind();
        debugCube->m_VertexArray.Bind();
        glm::mat4 model = glm::translate(glm::mat4(1.0f), debugCube->position + glm::vec3(0.5, 0.5, 0.5));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        shader->SetUniform("model", model);
        Engine::Renderer::RenderIndexed(debugCube->m_VertexArray);
    });
}
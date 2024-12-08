#include <Core/Allocator.hpp>
#include <Renderer/Skybox.hpp>
#include <Renderer/OpenGL/OpenGLSkybox.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/VertexLayout.hpp>
#include <Renderer/ShaderUniform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Engine
{
    void Skybox::Load(std::string_view cubemapName, const std::string& shaderPath,
                      const std::unordered_map<CubemapTextureFace, std::string>& Path)
    {
        m_Data = Allocator::Allocate<SkyboxData>();

        m_Data->m_Cubemap = CubemapTexture::Create(cubemapName, Path);
        m_Data->m_Shader = Shader::Load(shaderPath);
        m_Data->m_VertexArray = VertexArray::Create(6 * 6);

        float skyboxVertices[] = {                    //   Coordinates
                                  -1.0f, -1.0f, 1.0f, //        7--------6
                                  1.0f,  -1.0f, 1.0f, //       /|       /|
                                  1.0f,  -1.0f, -1.0f,//      4--------5 |
                                  -1.0f, -1.0f, -1.0f,//      | |      | |
                                  -1.0f, 1.0f,  1.0f, //      | 3------|-2
                                  1.0f,  1.0f,  1.0f, //      |/       |/
                                  1.0f,  1.0f,  -1.0f,//      0--------1
                                  -1.0f, 1.0f,  -1.0f};
        uint32_t skyboxIndices[] = {// Right
                                    1, 2, 6, 6, 5, 1,
                                    // Left
                                    0, 4, 7, 7, 3, 0,
                                    // Top
                                    4, 5, 6, 6, 7, 4,
                                    // Bottom
                                    0, 3, 2, 2, 1, 0,
                                    // Back
                                    0, 1, 5, 5, 4, 0,
                                    // Front
                                    3, 7, 6, 6, 2, 3};

        VertexLayout layout({{"Position", ShaderUniformType::Vec3}});

        m_Data->m_VertexArray->Bind();
        m_Data->m_VertexArray->AddVertexBuffer(layout, (float*) skyboxVertices, 8);
        m_Data->m_VertexArray->AddIndexBuffer(skyboxIndices, 36);
    }

    void Skybox::Destroy()
    {
        if (m_Data == nullptr) { return; }
        if (m_Data->m_Cubemap)
        {
            m_Data->m_Cubemap->Destroy();
            Allocator::Deallocate(m_Data->m_Cubemap);
            m_Data->m_Cubemap = nullptr;
        }
        if (m_Data->m_Shader)
        {
            m_Data->m_Shader->Destroy();
            Allocator::Deallocate(m_Data->m_Shader);
            m_Data->m_Shader = nullptr;
        }
        if (m_Data->m_VertexArray)
        {
            m_Data->m_VertexArray->Destroy();
            Allocator::Deallocate(m_Data->m_VertexArray);
            m_Data->m_VertexArray = nullptr;
        }
        Allocator::Deallocate(m_Data);
    }

    void Skybox::Reload() { m_Data->m_Shader->Reload(); }

    void Skybox::Update(float dt) { m_Data->m_Rotation += m_Data->m_RotationSpeed * dt; }

    RendererCommand Skybox::BindTexture(uint32_t slot) const
    {
        return RendererCommand([cubemap = m_Data->m_Cubemap, slot]() { cubemap->Bind(slot); });
    }

    RendererCommand Skybox::RenderCommand(Camera* camera) const
    {
        return RendererCommand([shader = m_Data->m_Shader, cubemap = m_Data->m_Cubemap, va = m_Data->m_VertexArray,
                                camera, rotation = m_Data->m_Rotation]() {
            Renderer::ChangeDepthFunction(DepthFunction::LEqual);
            shader->Bind();
            shader->SetUniform("skybox", 0);
            shader->SetUniform("camera.projection", camera->GetProjection());
            auto view = glm::mat4(glm::mat3(camera->GetView()));
            view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            shader->SetUniform("camera.view", view);

            cubemap->Bind(0);
            va->Bind();
            Renderer::RenderIndexed(va, va->IndexCount);
            Renderer::ChangeDepthFunction(DepthFunction::Less);
        });
    }

}// namespace Engine
#include "Skybox.hpp"
#include <Core/Allocator.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/VertexLayout.hpp>
#include <Renderer/ShaderUniform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>

namespace Engine
{
    Skybox* Skybox::Create(std::string_view cubemapName, const std::string& shaderPath,
                           const std::unordered_map<CubemapTextureFace, std::string>& Path)
    {
        Skybox* ptr;
        Allocate(Skybox, ptr);
        ptr->Load(cubemapName, shaderPath, Path);
        return ptr;
    }

    void Skybox::Load(std::string_view cubemapName, const std::string& shaderPath,
                      const std::unordered_map<CubemapTextureFace, std::string>& Path)
    {
        m_Cubemap = CubemapTexture::Create(cubemapName, Path);
        m_Shader = Shader::Load(shaderPath);
        m_VertexArray = VertexArray::Create(6 * 6);

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

        m_VertexArray->Bind();
        m_VertexArray->AddVertexBuffer(Engine::VertexBuffer::Create(layout, (float*) skyboxVertices, 8));
        m_VertexArray->AddIndexBuffer(Engine::IndexBuffer::Create(skyboxIndices, 36));
    }

    void Skybox::Destroy()
    {
        if (m_Cubemap)
        {
            m_Cubemap->Destroy();
            Deallocate(m_Cubemap);
            m_Cubemap = nullptr;
        }
        if (m_Shader)
        {
            m_Shader->Destroy();
            Deallocate(m_Shader);
            m_Shader = nullptr;
        }
        if (m_VertexArray)
        {
            m_VertexArray->Destroy();
            Deallocate(m_VertexArray);
            m_VertexArray = nullptr;
        }
    }

    void Skybox::Reload() { m_Shader->Reload(); }

    void Skybox::Draw(Camera* camera) const { Renderer::Submit(BeginRendering(camera)); }

    void Skybox::BindTexture(uint32_t slot) const { m_Cubemap->Bind(slot); }

    RendererCommand Skybox::BeginRendering(Camera* camera) const
    {
        return RendererCommand([shader = m_Shader, cubemap = m_Cubemap, va = m_VertexArray, camera]() {
            GLint depthFunc;
            glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
            glDepthFunc(GL_LEQUAL);
            shader->Bind();
            shader->SetUniform("skybox", 0);
            shader->SetUniform("camera.projection", camera->GetProjection());
            auto view = glm::mat4(glm::mat3(camera->GetView()));
            shader->SetUniform("camera.view", view);

            cubemap->Bind(0);
            va->Bind();
            glDrawElements(GL_TRIANGLES, va->IndexCount, GL_UNSIGNED_INT, nullptr);
            glDepthFunc(GL_LESS);
        });
    }

}// namespace Engine
#include "World.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/RendererCommand.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/hash.hpp>

namespace Engine
{
    void World::Init(TerrainGenerationSettings settings)
    {
        m_ChunkFactory.Init(settings);

        glm::ivec3 currentChunkPos = glm::ivec3(0, 0, 0);

        m_ChunksShapeData[currentChunkPos] = m_ChunkFactory.GenerateTerrainShapeData(currentChunkPos);
        m_ChunksBlockData[currentChunkPos] = m_ChunkFactory.Generate();
        m_ChunksMeshes[currentChunkPos] = ChunkMeshFactory::Generate(m_ChunksShapeData[currentChunkPos]);
    }

    void World::Destroy()
    {
        m_ChunkFactory.Destroy(m_ChunksBlockData[glm::ivec3(0, 0, 0)]);
        m_ChunkFactory.DestroyTerrainShapeData(m_ChunksShapeData[glm::ivec3(0, 0, 0)]);
        ChunkMeshFactory::Destroy(m_ChunksMeshes[glm::ivec3(0, 0, 0)]);
    }

    void World::OnUpdate(float dt) { m_Time += dt * 1000; };

    void World::Draw(Shader* shader) const
    {
        if (shader)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_Position);
            // model = glm::rotate(model, glm::radians(m_Time), glm::vec3(0.0f, 1.0f, 0.0f));

            const glm::ivec3 currentChunkPos = glm::ivec3(0, 0, 0);
            auto va = m_ChunksMeshes.at(currentChunkPos)->GetVertexArray();
            Renderer::Submit(GetRenderCommand(shader, va, model));
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
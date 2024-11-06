#include "World.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/RendererCommand.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Engine
{
    void World::Init(TerrainGenerationSettings settings)
    {
        m_ChunkFactory.Init(settings);

        glm::ivec3 currentChunkPos = glm::ivec3(0, 0, 0);
        uint32_t worldSize = 4;

        for (int z = 0; z < worldSize; z++)
        {
            for (int x = 0; x < worldSize; x++)
            {
                currentChunkPos = glm::ivec3(x, 0, z);

                m_ChunksShapeData[currentChunkPos] = m_ChunkFactory.GenerateTerrainShapeData(currentChunkPos);
                m_ChunksBlockData[currentChunkPos] = m_ChunkFactory.GenerateBlockData();

                m_ChunksMeshes[currentChunkPos] = ChunkMeshFactory::Generate(m_ChunksShapeData[currentChunkPos]);
            }
        }
    }

    void World::Destroy()
    {
        for (auto& [pos, shapeData]: m_ChunksShapeData) { m_ChunkFactory.DestroyTerrainShapeData(shapeData); }
        for (auto& [pos, blockData]: m_ChunksBlockData) { m_ChunkFactory.DestroyBlockData(blockData); }

        for (auto& [pos, mesh]: m_ChunksMeshes) { ChunkMeshFactory::Destroy(mesh); }
    }

    void World::OnUpdate(float dt) { m_Time += dt * 1000; };

    void World::Draw(Shader* shader) const
    {
        if (shader)
        {
            // model = glm::rotate(model, glm::radians(m_Time), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 model = glm::mat4(1.0f);
            model *= glm::scale(glm::mat4(1.0f), {1, 1, 1});

            for (auto& [pos, mesh]: m_ChunksMeshes)
            {
                auto va = mesh->GetVertexArray();

                Renderer::Submit(
                        GetRenderCommand(shader, va, model, glm::vec3{pos.x * CHUNK_SIZE, 0, pos.z * CHUNK_SIZE}));
            }
        }
    }

    RendererCommand World::GetRenderCommand(const Shader* const shader, const VertexArray* const va,
                                            const glm::mat4& model, glm::vec3 pos) const
    {
        return RendererCommand([shader, va, model, pos]() {
            shader->Bind();
            shader->SetUniform("model", model);
            va->Bind();
            shader->SetUniform("offset", pos);
            glDrawElements(GL_TRIANGLES, va->IndexCount, GL_UNSIGNED_INT, nullptr);
            va->Unbind();
        });
    }

    void World::Generate() {}
}// namespace Engine
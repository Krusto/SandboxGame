#include "World.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/RendererCommand.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>

namespace Engine
{
    void World::Init(TerrainGenerationSettings settings, std::filesystem::path texturesDirectory)
    {
        BlockRegistry::Init();
        std::unordered_map<uint32_t, std::string> texturePaths = BlockRegistry::AssignTexturesToIndices();

        for (auto& path: texturePaths) { path.second.insert(0, texturesDirectory.string() + "/"); }

        m_BlockTextures.Load("BlockTextures", texturePaths);

        m_ChunkFactory.Init(settings);

        glm::ivec3 currentChunkPos = glm::ivec3(0, 0, 0);
        uint32_t worldSize = 3;

        for (int z = 0; z < worldSize; z++)
        {
            for (int x = 0; x < worldSize; x++)
            {
                currentChunkPos = glm::ivec3(x, 0, z);

                m_Chunks[currentChunkPos] = m_ChunkFactory.GenerateChunk(currentChunkPos);
            }
        }
    }

    void World::Destroy()
    {
        for (auto& [pos, chunk]: m_Chunks) { m_ChunkFactory.DestroyChunk(chunk); }
        BlockRegistry::Destroy();
    }

    void World::OnUpdate(double dt) { m_Time += dt * 1000; };

    void World::Draw(Shader* shader) const
    {
        if (shader)
        {
            Renderer::Submit(BeginRenderingWorld(shader, &m_BlockTextures));

            for (auto& [pos, chunk]: m_Chunks)
            {
                Renderer::Submit(RenderChunk(shader, chunk.mesh->GetVertexArray(), chunk.mesh->GetBuffer(), pos));
            }
        }
    }

    RendererCommand World::BeginRenderingWorld(const Shader* shader, const TextureArray* textures) const
    {
        return RendererCommand([shader, textures]() {
            shader->Bind();
            textures->Bind();
        });
    }

    RendererCommand World::RenderChunk(const Shader* shader, const VertexArray* va, const StorageBuffer* blocks,
                                       glm::vec3 pos) const
    {
        return RendererCommand([shader, va, pos, blocks]() {
            blocks->Bind();
            glm::mat4 model(1.0);
            shader->SetUniform("model", model);
            shader->SetUniform("offset", glm::vec3{pos.x * CHUNK_SIZE, 0, pos.z * CHUNK_SIZE});
            va->Bind();
            glDrawElements(GL_TRIANGLES, va->IndexCount, GL_UNSIGNED_INT, nullptr);
            va->Unbind();
        });
    }

    void World::Generate() {}
}// namespace Engine
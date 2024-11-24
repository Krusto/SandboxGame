#include "World.hpp"
#include <Renderer/Renderer.hpp>
#include <Renderer/RendererCommand.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>
#include "BlockRegistry.hpp"

namespace Engine
{
    void World::Init(TerrainGenerationSettings settings, std::filesystem::path texturesDirectory)
    {
        m_Settings = settings;
        BlockRegistry::Init();
        std::unordered_map<uint32_t, std::string> texturePaths = BlockRegistry::AssignTexturesToIndices();

        for (auto& path: texturePaths) { path.second.insert(0, texturesDirectory.string() + "/"); }

        m_BlockTextures.Load("BlockTextures", texturePaths);

        ChunkFactory::Init(settings);

        glm::ivec3 currentChunkPos = glm::ivec3(0, 0, 0);
        uint32_t worldSize = settings.GenerationDistance;

        uint32_t maxChunksY = settings.maxTerrainHeight / CHUNK_SIZE;
        for (int z = 0; z < worldSize; z++)
        {
            for (int x = 0; x < worldSize; x++)
            {
                for (int y = 0; y < maxChunksY; y++)
                {
                    currentChunkPos = glm::ivec3(x, y, z);
                    ChunkFactory::ScheduleChunkForGeneration(currentChunkPos);
                }
            }
        }
    }

    void World::Destroy()
    {
        for (auto& [pos, chunk]: m_Chunks) { m_ChunkFactory.DestroyChunk(chunk); }
        BlockRegistry::Destroy();
    }

    void World::OnUpdate(double dt)
    {
        m_Time += dt * 1000;
        ChunkFactory::Update();
        ChunkFactory::UploadChunks();
        if (ChunkFactory::GeneratedChunksCount() > 0)
        {
            auto& generatedChunks = ChunkFactory::GetGeneratedChunks();
            m_Chunks.merge(generatedChunks);
            if (generatedChunks.size() > 0)
            {
                for (auto& [pos, chunk]: generatedChunks) { ChunkFactory::DestroyChunk(chunk); }
                generatedChunks.clear();
            }
        }
    };

    RendererCommand World::RenderWorldCommand(Shader* shader) const
    {
        return RendererCommand([shader, this]() {
            m_BlockTextures.Bind();
            for (auto& [pos, chunk]: m_Chunks)
            {
                auto& mesh = chunk.mesh;
                if (mesh->GetVertexArray() != nullptr)
                {
                    mesh->GetBuffer()->Bind();
                    glm::mat4 model(1.0);
                    shader->SetUniform("model", model);
                    shader->SetUniform("offset", glm::vec3{pos.x * CHUNK_SIZE, pos.y * CHUNK_SIZE, pos.z * CHUNK_SIZE});
                    mesh->GetVertexArray()->Bind();
                    glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->IndexCount, GL_UNSIGNED_INT, nullptr);
                    mesh->GetVertexArray()->Unbind();
                }
            }
        });
    }

    void World::Generate() {}

    void World::Reload()
    {
        ChunkFactory::Reload();
        for (auto& [pos, chunk]: m_Chunks) { m_ChunkFactory.DestroyChunk(chunk); }
        m_Chunks.clear();

        uint32_t worldSize = m_Settings.GenerationDistance;

        uint32_t maxChunksY = m_Settings.maxTerrainHeight / CHUNK_SIZE;
        for (int z = 0; z < worldSize; z++)
        {
            for (int x = 0; x < worldSize; x++)
            {
                for (int y = 0; y < maxChunksY; y++)
                {
                    auto currentChunkPos = glm::ivec3(x, y, z);
                    ChunkFactory::ScheduleChunkForGeneration(currentChunkPos);
                }
            }
        }
    }

    uint8_t World::GetBlock(glm::ivec3 position) const
    {
        uint8_t block = BlockType::AIR;

        glm::ivec3 chunkPos = glm::ivec3(position.x / CHUNK_SIZE, position.y / CHUNK_SIZE, position.z / CHUNK_SIZE);
        for (auto& [pos, chunk]: m_Chunks)
        {
            if (pos == chunkPos)
            {
                glm::ivec3 localPos = position - glm::ivec3(chunkPos.x * CHUNK_SIZE, chunkPos.y * CHUNK_SIZE,
                                                            chunkPos.z * CHUNK_SIZE);
                block = chunk.blockData->GetBlock(localPos);
                return block;
            }
        }
        return 0;
    }
}// namespace Engine
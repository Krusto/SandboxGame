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
        BlockRegistry::Init();
        std::unordered_map<uint32_t, std::string> texturePaths = BlockRegistry::AssignTexturesToIndices();

        for (auto& path: texturePaths) { path.second.insert(0, texturesDirectory.string() + "/"); }

        m_BlockTextures.Load("BlockTextures", texturePaths);

        ChunkFactory::Init(settings);

        glm::ivec3 currentChunkPos = glm::ivec3(0, 0, 0);
        uint32_t worldSize = 1;

        for (int z = 0; z < worldSize; z++)
        {
            for (int x = 0; x < worldSize; x++)
            {
                for (int y = 0; y < worldSize; y++)
                {
                    currentChunkPos = glm::ivec3(x, y, z);
                    ChunkFactory::ScheduleChunkForGeneration(currentChunkPos);
                    // m_Chunks[currentChunkPos] = Chunk();
                    // m_Chunks[currentChunkPos].terrainShape =
                    //         ChunkFactory::GenerateTerrainShape(&settings, currentChunkPos);
                    // m_Chunks[currentChunkPos].blockData = ChunkFactory::GenerateBlockData(
                    //         &settings, m_Chunks[currentChunkPos].terrainShape, currentChunkPos);
                    // LOG_INFO("Generating %s\n", glm::to_string(currentChunkPos).c_str());
                    // m_MeshFutures.push_back(std::make_pair(currentChunkPos,std::async(std::launch::async, ChunkFactory::GenerateChunkMesh,
                    //                                             m_Chunks[currentChunkPos].blockData)));
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

        auto generatedChunks = ChunkFactory::GetChunks();

        for (auto& [pos, chunk]: generatedChunks)
        {
            if (m_Meshes.find(pos) == m_Meshes.end())
            {
                m_Meshes[pos] = chunk.mesh;
                ChunkMesh::UploadData(m_Meshes[pos]);
            }
            m_Chunks[pos] = chunk;
        }
        // for (uint32_t i = 0; i < m_MeshFutures.size(); i++)
        // {
        //     glm::ivec3 pos = m_MeshFutures[i].first;
        //     std::future<ChunkMesh*>* mesh = &m_MeshFutures[i].second;

        //     std::string p = glm::to_string(pos);
        //     if (mesh->wait_for(std::chrono::seconds(0)) == std::future_status::ready)
        //     {
        //         m_Meshes[pos] = mesh->get();
        //         ChunkMesh::UploadData(m_Meshes[pos]);
        //         m_MeshFutures.erase(m_MeshFutures.begin() + i);
        //     }
        // }
    };

    void World::Draw(Shader* shader) const
    {
        if (shader)
        {
            Renderer::Submit(BeginRenderingWorld(shader, &m_BlockTextures));

            for (auto& [pos, mesh]: m_Meshes)
            {
                Renderer::Submit(RenderChunk(shader, mesh->GetVertexArray(), mesh->GetBuffer(), pos));
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
            shader->SetUniform("offset", glm::vec3{pos.x * CHUNK_SIZE, pos.y * CHUNK_SIZE, pos.z * CHUNK_SIZE});
            va->Bind();
            glDrawElements(GL_TRIANGLES, va->IndexCount, GL_UNSIGNED_INT, nullptr);
            va->Unbind();
        });
    }

    void World::Generate() {}

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
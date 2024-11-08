#pragma once
#include <glad/glad.h>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <Renderer/Renderer.hpp>
#include "ChunkFactory.hpp"

namespace Engine
{
    class World
    {
    public:
        World() = default;
        ~World() = default;

    public:
        void Init(TerrainGenerationSettings settings, std::filesystem::path texturesDirectory);

        void Destroy();

        void OnUpdate(float dt);

        void Draw(Shader* shader) const;

        void Generate();

    private:
        RendererCommand BeginRenderingWorld(const Shader* shader, const TextureArray* textures) const;

        RendererCommand RenderChunk(const Shader* shader, const VertexArray* va, glm::vec3 pos) const;

    private:
        Engine::TextureArray m_BlockTextures;

        ChunkFactory m_ChunkFactory;

        std::unordered_map<glm::ivec3, Chunk> m_Chunks;

        float m_Time;
    };


}// namespace Engine
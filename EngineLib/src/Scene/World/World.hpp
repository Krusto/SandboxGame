#pragma once
#include <glad/glad.h>
#include <glm/gtx/hash.hpp>
#include <unordered_map>
#include <Renderer/VertexArray.hpp>
#include <Renderer/Shader.hpp>
#include <Renderer/RendererCommand.hpp>
#include "ChunkFactory.hpp"
#include "ChunkMeshFactory.hpp"

namespace Engine
{
    class World
    {
    public:
        World() = default;
        ~World() = default;

    public:
        void Init(TerrainGenerationSettings settings);

        void Destroy();

        void OnUpdate(float dt);

        void Draw(Shader* shader) const;

        void Generate();

    private:
        RendererCommand GetRenderCommand(const Shader* const shader, const VertexArray* const va,
                                         const glm::mat4& model) const;

    private:
        glm::vec3 m_Position{0.0f, 0.0f, 0.0f};
        float m_Time;

        ChunkFactory m_ChunkFactory;
        std::unordered_map<glm::ivec3, ChunkData*> m_ChunksBlockData;
        std::unordered_map<glm::ivec3, TerrainShapeData*> m_ChunksShapeData;
        std::unordered_map<glm::ivec3, ChunkMesh*> m_ChunksMeshes;
    };


}// namespace Engine
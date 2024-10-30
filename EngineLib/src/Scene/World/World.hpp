#pragma once
#include <glad/glad.h>
#include <Renderer/VertexArray.hpp>
#include <Renderer/Shader.hpp>
#include <Renderer/RendererCommand.hpp>

namespace Engine
{
    class World
    {
    public:
        World() = default;
        ~World() = default;

    public:
        void Init(int seed);

        void Destroy();

        void OnUpdate(float dt);

        void Draw(Shader* shader) const;

        void Generate();

    private:
        RendererCommand GetRenderCommand(const Shader* const shader, const VertexArray* const va,
                                         const glm::mat4& model) const;

    private:
        VertexArray* m_VertexArray{};
        int m_Seed{};
        glm::vec3 m_Position{0.0f, 0.0f, 0.0f};
        float m_Time;
    };


}// namespace Engine
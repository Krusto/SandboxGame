#pragma once
#include <glad/glad.h>
#include <Renderer/VertexArray.hpp>
#include <Renderer/Shader.hpp>

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

        void GenerateWorld();

    private:
        VertexArray* m_VertexArray;
        int m_Seed = 0;
    };
}// namespace Engine
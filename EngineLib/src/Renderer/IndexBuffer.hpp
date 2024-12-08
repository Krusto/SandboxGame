#pragma once
#include <cstdint>
#include <Renderer/RendererCore.hpp>

namespace Engine
{
    class VertexArray;
    struct IndexBufferData;

    class IndexBuffer
    {
    public:
        static IndexBuffer Create(VertexArray* va, const uint32_t* data, uint32_t length);

    public:
        void Init(VertexArray* va, const uint32_t* data, uint32_t length);
        void Destroy();
        void Bind() const;
        uint32_t GetID() const;

    public:
        uint32_t indexCount{};

    private:
        IndexBufferData* m_Data{};
    };
}// namespace Engine

#ifdef RENDERER_BUILD_DLL
class __declspec(dllexport) Engine::IndexBuffer;
#else
class __declspec(dllimport) Engine::IndexBuffer;
#endif
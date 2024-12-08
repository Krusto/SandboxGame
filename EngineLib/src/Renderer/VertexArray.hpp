#pragma once
#include <cstdint>
#include <Core/Ref.hpp>
#include <Renderer/RendererCore.hpp>
#include <Renderer/VertexBuffer.hpp>
#include <Renderer/IndexBuffer.hpp>
#include <Renderer/VertexLayout.hpp>

namespace Engine
{

    class VertexArray
    {
    public:
        static VertexArray* Create(uint32_t indexCount);

    public:
        virtual ~VertexArray() = default;

        virtual void Init(uint32_t indexCount) = 0;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void Destroy() = 0;

        virtual void AddVertexBuffer(const VertexLayout& layout, float* data, uint32_t length) = 0;

        virtual void AddIndexBuffer(const uint32_t* data, uint32_t length) = 0;

    public:
        uint32_t id() const { return m_ID; }


    public:
        uint32_t IndexCount{};

    protected:
        VertexBuffer m_VertexBuffer{};
        IndexBuffer m_IndexBuffer{};
        uint32_t m_ID{};
    };
}// namespace Engine


#ifdef RENDERER_BUILD_DLL
class __declspec(dllexport) Engine::VertexArray;
#else
class __declspec(dllimport) Engine::VertexArray;
#endif
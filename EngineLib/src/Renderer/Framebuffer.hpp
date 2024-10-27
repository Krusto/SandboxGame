#pragma once
#include <Core/Ref.hpp>
#include <Renderer/Viewport.hpp>

namespace Engine
{
    class Framebuffer: public RefCounted
    {
    public:
        virtual ~Framebuffer() = default;

        static Ref<Framebuffer> Create(uint32_t width, uint32_t height);

        virtual uint32_t GetID() = 0;

        virtual uint32_t& GetColorAttachmentID() = 0;

        virtual void Resize(uint32_t width, uint32_t height) = 0;

        virtual void Destroy() = 0;

        virtual void Bind() = 0;

        virtual void Unbind() = 0;

        virtual uint32_t width() = 0;

        virtual uint32_t height() = 0;

        virtual ViewportSize GetViewportSize() = 0;
    };
}// namespace Engine
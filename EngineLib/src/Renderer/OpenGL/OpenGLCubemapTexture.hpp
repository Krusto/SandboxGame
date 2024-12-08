#pragma once
#include <Renderer/CubemapTexture.hpp>
#include <string>
#include <string_view>

namespace Engine
{
    class OpenGLCubemapTexture: public CubemapTexture
    {
    public:
        OpenGLCubemapTexture() = default;
        ~OpenGLCubemapTexture() = default;

    public:
        virtual void Load(std::string_view cubemapName,
                          const std::unordered_map<CubemapTextureFace, std::string>& Path) override;
        virtual void Destroy() override;

    public:
        virtual std::string_view GetName() const override;
        virtual uint32_t ID() const override;
        virtual uint32_t Width() const override;
        virtual uint32_t Height() const override;
        virtual uint32_t Channels() const override;
        virtual void Bind(uint32_t slot = 0) const override;

    protected:
        CubemapTextureSpec m_Spec;
        uint32_t m_ID;
    };
}// namespace Engine
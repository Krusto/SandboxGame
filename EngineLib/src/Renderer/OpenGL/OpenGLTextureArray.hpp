#pragma once

#include <Renderer/TextureArray.hpp>

namespace Engine
{

    class OpenGLTextureArray: public TextureArray
    {
    public:
        OpenGLTextureArray() = default;
        ~OpenGLTextureArray() = default;

    public:
        virtual void Load(std::string_view arrayName, std::initializer_list<std::string> Path) override;
        virtual void Load(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path) override;
        virtual void Bind(uint32_t slot = 0) const override;
        virtual void Destroy() override;

    public:
        virtual std::string_view GetName() const override;
        virtual uint32_t ID() const override;
        virtual uint32_t Width() const override;
        virtual uint32_t Height() const override;
        virtual uint32_t Channels() const override;

    protected:
        TextureArraySpec m_Spec;
        uint32_t m_ID;
    };
}// namespace Engine
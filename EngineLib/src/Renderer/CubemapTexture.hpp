#pragma once

#include <string>
#include <Core/Ref.hpp>

namespace Engine
{

    enum class CubemapTextureFace : int
    {
        Right = 0,
        Left,
        Top,
        Bottom,
        Front,
        Back
    };

    struct CubemapTextureSpec {
        std::string_view Name;
        std::string_view WorkingDirectory;
        int32_t Width;
        int32_t Height;
        int32_t Channels;
    };

    class CubemapTexture
    {
    public:
        CubemapTexture() = default;
        ~CubemapTexture() = default;

    public:
        static CubemapTexture* Create(std::string_view cubemapName,
                                      const std::unordered_map<CubemapTextureFace, std::string>& Path);

        void Load(std::string_view cubemapName, const std::unordered_map<CubemapTextureFace, std::string>& Path);

        void Destroy();

    public:
        std::string_view GetName() const;
        uint32_t ID() const;
        uint32_t Width() const;
        uint32_t Height() const;
        uint32_t Channels() const;
        void Bind(uint32_t slot = 0) const;

    protected:
        CubemapTextureSpec m_Spec;
        uint32_t m_ID;
    };
}// namespace Engine
#pragma once

#include <string>
#include <Core/Ref.hpp>
#include <Renderer/Predefines.hpp>

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
        std::string_view Name{};
        std::string_view WorkingDirectory{};
        int32_t Width{};
        int32_t Height{};
        int32_t Channels{};
    };

    struct CubemapTextureData;

    class EXPORT_RENDERER CubemapTexture
    {
    public:
        CubemapTexture() = default;
        ~CubemapTexture() = default;

        static CubemapTexture Create(std::string_view cubemapName,
                                     const std::unordered_map<CubemapTextureFace, std::string>& Path);

    public:
        void Load(std::string_view cubemapName, const std::unordered_map<CubemapTextureFace, std::string>& Path);
        void Destroy();
        std::string_view GetName() const;
        uint32_t ID() const;
        uint32_t Width() const;
        uint32_t Height() const;
        uint32_t Channels() const;
        void Bind(uint32_t slot = 0) const;

    private:
        CubemapTextureData* m_Data{};
    };
}// namespace Engine
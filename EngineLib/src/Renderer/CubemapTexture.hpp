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
        std::string_view Name;
        std::string_view WorkingDirectory;
        int32_t Width;
        int32_t Height;
        int32_t Channels;
    };

    class EXPORT_RENDERER CubemapTexture
    {
    public:
        CubemapTexture() = default;
        virtual ~CubemapTexture() = default;

        static CubemapTexture* Create(std::string_view cubemapName,
                                      const std::unordered_map<CubemapTextureFace, std::string>& Path);

    public:
        virtual void Load(std::string_view cubemapName,
                          const std::unordered_map<CubemapTextureFace, std::string>& Path) = 0;
        virtual void Destroy() = 0;

    public:
        virtual std::string_view GetName() const = 0;
        virtual uint32_t ID() const = 0;
        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;
        virtual uint32_t Channels() const = 0;
        virtual void Bind(uint32_t slot = 0) const = 0;
    };
}// namespace Engine
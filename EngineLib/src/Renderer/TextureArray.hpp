#pragma once

#include <string>
#include <Core/Core.hpp>
#include <Core/Ref.hpp>
#include <Renderer/RendererCore.hpp>

namespace Engine
{
    struct TextureArraySpec {
        std::string_view Name;
        std::string_view Path;
        int32_t Width;
        int32_t Height;
        int32_t Channels;
    };

    class TextureArray
    {
    public:
        TextureArray() = default;
        virtual ~TextureArray() = default;

        static TextureArray* Create();

    public:
        virtual void Load(std::string_view arrayName, std::initializer_list<std::string> Path) = 0;
        virtual void Load(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path) = 0;
        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void Destroy() = 0;

    public:
        virtual std::string_view GetName() const = 0;
        virtual uint32_t ID() const = 0;
        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;
        virtual uint32_t Channels() const = 0;
    };
}// namespace Engine

#ifdef RENDERER_BUILD_DLL
class __declspec(dllexport) Engine::TextureArray;
#else
class __declspec(dllimport) Engine::TextureArray;
#endif
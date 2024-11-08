#pragma once

#include <string>
#include <Core/Ref.hpp>

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
        ~TextureArray() = default;

    public:
        void Load(std::string_view arrayName, std::initializer_list<std::string> Path);
        void Load(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path);

        void Bind(uint32_t slot = 0) const;

        void Destroy();

    public:
        std::string_view GetName() const;
        uint32_t ID() const;
        uint32_t Width() const;
        uint32_t Height() const;
        uint32_t Channels() const;

    protected:
        TextureArraySpec m_Spec;
        uint32_t m_ID;
    };
}// namespace Engine
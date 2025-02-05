#pragma once

#include <string>
#include <Core/Core.hpp>
#include <Renderer/Predefines.hpp>

namespace Engine
{
    struct TextureArrayData;

    class TextureArray
    {
    public:
        TextureArray() = default;
        ~TextureArray() = default;

    public:
        static TextureArray Create();

        void Load(std::string_view arrayName, std::initializer_list<std::string> Path);
        void Load(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path);
        void Bind(uint32_t slot = 0) const;
        void Destroy();
        std::string_view GetName() const;
        uint32_t ID() const;
        uint32_t Width() const;
        uint32_t Height() const;
        uint32_t Channels() const;

        bool IsValid() const { return m_Data != nullptr; }

    private:
        TextureArrayData* m_Data{};
    };
}// namespace Engine
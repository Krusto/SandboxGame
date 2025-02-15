#pragma once

#include <string>
#include <Core/Ref.hpp>
#include <Core/STL/Containers.hpp>
#include <Renderer/Shared/CubemapTextureSpec.hpp>

namespace Engine
{

    struct CubemapTextureData;

    class CubemapTexture
    {
    public:
        CubemapTexture() = default;
        ~CubemapTexture() = default;

        static CubemapTexture Create(std::string_view cubemapName,
                                     const Vector<Pair<CubemapTextureFace, const char*>>* Path);

    public:
        void Load(std::string_view cubemapName, const Vector<Pair<CubemapTextureFace, const char*>>* Path);
        void Destroy();

        const CubemapTextureSpec* GetSpec() const;
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
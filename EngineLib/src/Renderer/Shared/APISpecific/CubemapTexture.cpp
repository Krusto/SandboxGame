#include "CubemapTexture.hpp"

namespace Engine
{
    CubemapTexture CubemapTexture::Create(std::string_view cubemapName,
                                          const std::unordered_map<CubemapTextureFace, std::string>& Path)
    {
        CubemapTexture ptr;
        ptr.Load(cubemapName, Path);
        return ptr;
    }


}// namespace Engine
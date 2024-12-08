#include "CubemapTexture.hpp"
#include <Core/Allocator.hpp>

#include "OpenGL/OpenGLCubemapTexture.hpp"

namespace Engine
{
    CubemapTexture* CubemapTexture::Create(std::string_view cubemapName,
                                           const std::unordered_map<CubemapTextureFace, std::string>& Path)
    {
        auto ptr = Allocator::Allocate<OpenGLCubemapTexture>();
        ptr->Load(cubemapName, Path);
        return ptr;
    }


}// namespace Engine
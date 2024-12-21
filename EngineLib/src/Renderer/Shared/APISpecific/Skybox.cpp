#include "Skybox.hpp"

namespace Engine
{
    Skybox Skybox::Create(std::string_view cubemapName, const std::string& shaderPath,
                          const std::unordered_map<CubemapTextureFace, std::string>& Path)
    {
        Skybox ptr;
        ptr.Load(cubemapName, shaderPath, Path);
        return ptr;
    }

}// namespace Engine
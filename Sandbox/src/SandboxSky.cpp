#include "SandboxSky.hpp"
#include <Core/STL/Containers.hpp>

void SandboxSky::Create(std::string_view shaderPath, std::string_view texturePath, std::string_view cubemapName)
{
    std::string skyboxName(cubemapName);
    std::string skyboxPath = std::string(texturePath) + "/" + skyboxName;
    std::string skyboxPath_right = skyboxPath + "_right.png";
    std::string skyboxPath_left = skyboxPath + "_left.png";
    std::string skyboxPath_top = skyboxPath + "_top.png";
    std::string skyboxPath_bottom = skyboxPath + "_bottom.png";
    std::string skyboxPath_front = skyboxPath + "_front.png";
    std::string skyboxPath_back = skyboxPath + "_back.png";
    using Engine::CubemapTextureFace;
    using Engine::Pair;
    using Engine::Vector;
    Vector<Pair<CubemapTextureFace, const char*>> faces(
            {Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Right, skyboxPath_right.c_str()),
             Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Left, skyboxPath_left.c_str()),
             Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Top, skyboxPath_top.c_str()),
             Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Bottom, skyboxPath_bottom.c_str()),
             Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Front, skyboxPath_front.c_str()),
             Pair<CubemapTextureFace, const char*>(CubemapTextureFace::Back, skyboxPath_back.c_str())});
    std::string shader(shaderPath);
    m_Skybox = Engine::Skybox::Create(skyboxName, shader, &faces);
}
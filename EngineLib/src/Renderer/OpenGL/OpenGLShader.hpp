#pragma once
#include <glad/glad.h>

#include <Core/Log.hpp>

#include <Renderer/Shader.hpp>
#include <Renderer/ShaderUniform.hpp>

namespace Engine
{
    struct ShaderData {
        uint32_t rendererID = 0;
        bool loaded = false;
        bool compiled = false;

        std::string name, assetPath;

        std::unordered_map<GLenum, std::string> shaderSource;
        std::vector<ShaderUniformBlockLayout> uniformBlocks;
    };

}// namespace Engine
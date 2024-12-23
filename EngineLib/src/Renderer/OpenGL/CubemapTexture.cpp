#include <Renderer/Shared/APISpecific/CubemapTexture.hpp>
#include <Core/Allocator.hpp>
#include <filesystem>
#include <glad/glad.h>

//TODO : remove
#include "stb_image.h"

namespace Engine
{
    struct CubemapTextureData {
        CubemapTextureSpec spec{};
        uint32_t id{};
    };

    void CubemapTexture::Load(std::string_view cubemapName,
                              const std::unordered_map<CubemapTextureFace, std::string>& Path)
    {
        if (m_Data == nullptr) { m_Data = Engine::Allocator::Allocate<CubemapTextureData>(); }
        m_Data->spec.Name = cubemapName;
        for (auto& [face, file]: Path)
        {
            if (!std::filesystem::exists(file))
            {
                LOG_ERROR("%s does not exist!\n", file.c_str());
                return;
            }
        }
        glGenTextures(1, &m_Data->id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Data->id);
        bool first = true;
        uint32_t index = 0;
        for (auto& [face, file]: Path)
        {
            stbi_uc* data = nullptr;
            {
                data = stbi_load(file.data(), &m_Data->spec.Width, &m_Data->spec.Height, &m_Data->spec.Channels, 0);
            }
            if (nullptr == data) { LOG_ERROR("Can not load %s\n", file.c_str()); }
            else { LOG_INFO("Loaded %s\n", file.c_str()); }

            GLenum internalFormat = 0, dataFormat = 0;
            if (m_Data->spec.Channels == 4)
            {
                internalFormat = GL_RGBA;
                dataFormat = GL_RGBA;
            }
            else if (m_Data->spec.Channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            else if (m_Data->spec.Channels == 2)
            {
                internalFormat = GL_RG;
                dataFormat = GL_RG;
            }
            else if (m_Data->spec.Channels == 1)
            {
                internalFormat = GL_RED;
                dataFormat = GL_RED;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, internalFormat, m_Data->spec.Width,
                         m_Data->spec.Height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
            index++;
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void CubemapTexture::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Data->id);
    }

    void CubemapTexture::Destroy()
    {
        if (m_Data == nullptr) { return; }
        glDeleteTextures(1, &m_Data->id);
        Allocator::Deallocate(m_Data);
        m_Data = nullptr;
    }

    std::string_view CubemapTexture::GetName() const { return m_Data->spec.Name; }

    uint32_t CubemapTexture::ID() const { return m_Data->id; }

    uint32_t CubemapTexture::Width() const { return m_Data->spec.Width; }

    uint32_t CubemapTexture::Height() const { return m_Data->spec.Height; }

    uint32_t CubemapTexture::Channels() const { return m_Data->spec.Channels; }
}// namespace Engine
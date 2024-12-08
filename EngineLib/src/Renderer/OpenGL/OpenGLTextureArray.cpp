#include "OpenGLTextureArray.hpp"
#include <filesystem>
#include <algorithm>

#include <glad/glad.h>

#include <Core/Log.hpp>
//TODO: remove
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Engine
{

    void OpenGLTextureArray::Load(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path)
    {
        m_Spec.Name = arrayName;
        for (auto& [index, file]: Path)
        {
            if (!std::filesystem::exists(file))
            {
                LOG_ERROR("%s does not exist!\n", file.c_str());
                return;
            }
        }

        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
        bool first = true;
        for (auto& [textureIndex, file]: Path)
        {
            stbi_uc* data = nullptr;
            {
                data = stbi_load(file.data(), &m_Spec.Width, &m_Spec.Height, &m_Spec.Channels, 0);
            }
            if (nullptr == data) { LOG_ERROR("Can not load %s\n", file.c_str()); }
            else { LOG_INFO("Loaded %s\n", file.c_str()); }

            GLenum internalFormat = 0, dataFormat = 0;
            if (m_Spec.Channels == 4)
            {
                internalFormat = GL_RGBA;
                dataFormat = GL_RGBA;
            }
            else if (m_Spec.Channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            else if (m_Spec.Channels == 2)
            {
                internalFormat = GL_RG;
                dataFormat = GL_RG;
            }
            else if (m_Spec.Channels == 1)
            {
                internalFormat = GL_RED;
                dataFormat = GL_RED;
            }

            if (first)
            {
                glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, m_Spec.Width, m_Spec.Height, Path.size(), 0,
                             dataFormat, GL_UNSIGNED_BYTE, nullptr);
                first = false;
            }

            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textureIndex, m_Spec.Width, m_Spec.Height, 1, dataFormat,
                            GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

            stbi_image_free(data);
        }
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    void OpenGLTextureArray::Load(std::string_view arrayName, std::initializer_list<std::string> Path)
    {
        m_Spec.Name = arrayName;
        for (auto& file: Path)
        {
            if (!std::filesystem::exists(file))
            {
                LOG_ERROR("%s does not exist!", file.data());
                return;
            }
        }

        glGenTextures(1, &m_ID);
        glActiveTexture(GL_TEXTURE0 + Path.size());
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
        bool first = true;
        int index = 0;
        for (auto file: Path)
        {
            stbi_uc* data = nullptr;
            {
                data = stbi_load(file.data(), &m_Spec.Width, &m_Spec.Height, &m_Spec.Channels, 0);
            }

            GLenum internalFormat = 0, dataFormat = 0;
            if (m_Spec.Channels == 4)
            {
                internalFormat = GL_RGBA;
                dataFormat = GL_RGBA;
            }
            else if (m_Spec.Channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            else if (m_Spec.Channels == 2)
            {
                internalFormat = GL_RG;
                dataFormat = GL_RG;
            }
            else if (m_Spec.Channels == 1)
            {
                internalFormat = GL_RED;
                dataFormat = GL_RED;
            }

            if (first)
            {
                glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, m_Spec.Width, m_Spec.Height, Path.size(), 0,
                             dataFormat, GL_UNSIGNED_BYTE, nullptr);
                first = false;
            }

            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, m_Spec.Width, m_Spec.Height, 1, dataFormat,
                            GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

            stbi_image_free(data);
            index++;
        }
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    void OpenGLTextureArray::Bind(uint32_t slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_ID);
    }

    void OpenGLTextureArray::Destroy() { glDeleteTextures(1, &m_ID); }

    std::string_view OpenGLTextureArray::GetName() const { return m_Spec.Name; }

    uint32_t OpenGLTextureArray::ID() const { return m_ID; };

    uint32_t OpenGLTextureArray::Width() const { return m_Spec.Width; }

    uint32_t OpenGLTextureArray::Height() const { return m_Spec.Height; }

    uint32_t OpenGLTextureArray::Channels() const { return m_Spec.Channels; }
}// namespace Engine

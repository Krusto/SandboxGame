#include <Renderer/Predefines.hpp>
#include <Renderer/Shared/CubemapTextureSpec.hpp>
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

    EXPORT_RENDERER void CubemapTextureLoad(void** data, void* cubemapName, void* paths)
    {

        if (cubemapName == nullptr || paths == nullptr) { return; }
        if (*data == nullptr) { *data = Engine::Allocator::Allocate<CubemapTextureData>(); }
        CubemapTextureData* m_Data = static_cast<CubemapTextureData*>(*data);

        for (auto& [face, file]: asUnorderedMap(paths, CubemapTextureFace, std::string))
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
        for (auto& [face, file]: asUnorderedMap(paths, CubemapTextureFace, std::string))
        {
            stbi_uc* image_data = nullptr;
            {
                image_data =
                        stbi_load(file.data(), &m_Data->spec.Width, &m_Data->spec.Height, &m_Data->spec.Channels, 0);
            }
            if (nullptr == image_data) { LOG_ERROR("Can not load %s\n", file.c_str()); }
            else { LOG_INFO("Loaded %s\n", file.c_str()); }

            GLenum internalFormat = 0, dataFormat = 0;
            if (m_Data->spec.Channels == 4)
            {
                internalFormat = GL_RGBA;
                dataFormat = GL_RGBA;
            }
            else if (m_Data->spec.Channels == 3)
            {
                internalFormat = GL_RGB;
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
                         m_Data->spec.Height, 0, dataFormat, GL_UNSIGNED_BYTE, image_data);
            stbi_image_free(image_data);
            index++;
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    EXPORT_RENDERER void CubemapTextureBind(void* data, unsigned int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, asTPtr(data, CubemapTextureData)->id);
    }

    EXPORT_RENDERER void CubemapTextureDestroy(void** data)
    {
        if (data == nullptr) { return; }
        glDeleteTextures(1, &(asTPtr(data, CubemapTextureData)->id));
        Allocator::Deallocate(*data);
        *data = nullptr;
    }

    EXPORT_RENDERER void* CubemapTextureGetSpec(void* data) { return (void*) &asTPtr(data, CubemapTextureData)->spec; }

    EXPORT_RENDERER int CubemapTextureGetID(void* data) { return asTPtr(data, CubemapTextureData)->id; }

}// namespace Engine
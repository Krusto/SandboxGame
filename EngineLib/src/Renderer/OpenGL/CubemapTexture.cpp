#include <Renderer/Predefines.hpp>
#include <Renderer/Shared/CubemapTextureSpec.hpp>
#include <Core/Allocator.hpp>
#include <glad/glad.h>

#include "StructDefinitions.hpp"
//TODO : remove
#include "stb_image.h"
#include <Core/STL/CVector.h>
#include <Core/STL/CPair.h>
#include <Core/STL/CFilesystem.h>

#ifdef __cplusplus
namespace Engine
{
#endif
    DeclareCPair(uint8_t, const char*, CubemapTexturePathT);

    EXPORT_RENDERER CubemapTextureData* CubemapTextureLoad(const char* cubemapName, const CVectorT* paths)
    {

        if (cubemapName == nullptr || paths == nullptr) { return nullptr; }
        CubemapTextureData* data = Allocator::Allocate<CubemapTextureData>();
        for (uint32_t i = 0; i < cvector_length(paths); i++)
        {
            const CubemapTexturePathT* path = (const CubemapTexturePathT*) cvector_get_ptr_const(paths, i);
            CubemapTextureFace face = (CubemapTextureFace) path->first;
            const char* file = path->second;

            if (!FileExists(file))
            {
                LOG_ERROR("%s does not exist!\n", file);
                return nullptr;
            }
        }

        glGenTextures(1, &data->id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, data->id);
        bool first = true;
        uint32_t index = 0;
        for (uint32_t i = 0; i < cvector_length(paths); i++)
        {
            CubemapTexturePathT path = *(const CubemapTexturePathT*) cvector_get_ptr_const(paths, i);
            CubemapTextureFace face = (CubemapTextureFace) path.first;
            const char* file = path.second;

            stbi_uc* image_data = nullptr;
            {
                image_data = stbi_load(file, &data->spec.Width, &data->spec.Height, &data->spec.Channels, 0);
            }
            if (nullptr == image_data) { LOG_ERROR("Can not load %s\n", file); }
            else { LOG_INFO("Loaded %s\n", file); }

            GLenum internalFormat = 0, dataFormat = 0;
            if (data->spec.Channels == 4)
            {
                internalFormat = GL_RGBA;
                dataFormat = GL_RGBA;
            }
            else if (data->spec.Channels == 3)
            {
                internalFormat = GL_RGB;
                dataFormat = GL_RGB;
            }
            else if (data->spec.Channels == 2)
            {
                internalFormat = GL_RG;
                dataFormat = GL_RG;
            }
            else if (data->spec.Channels == 1)
            {
                internalFormat = GL_RED;
                dataFormat = GL_RED;
            }

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, internalFormat, data->spec.Width, data->spec.Height,
                         0, dataFormat, GL_UNSIGNED_BYTE, image_data);
            stbi_image_free(image_data);
            index++;
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        return data;
    }

    EXPORT_RENDERER void CubemapTextureBind(CubemapTextureData* data, unsigned int slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, data->id);
    }

    EXPORT_RENDERER void CubemapTextureDestroy(CubemapTextureData** data)
    {
        if (data == nullptr) { return; }
        glDeleteTextures(1, &(*data)->id);
        Allocator::Deallocate(*data);
        *data = nullptr;
    }

    EXPORT_RENDERER CubemapTextureSpec* CubemapTextureGetSpec(CubemapTextureData* data) { return &data->spec; }

    EXPORT_RENDERER int CubemapTextureGetID(CubemapTextureData* data) { return data->id; }
#ifdef __cplusplus
}// namespace Engine
#endif
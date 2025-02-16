#include <Renderer/Predefines.hpp>
#include <Renderer/OpenGL/ExportedFunctions.hpp>
#include <Renderer/OpenGL/StructDefinitions.hpp>
#include <Core/Log.hpp>
#include <Core/Allocator.hpp>

//TODO: remove
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

#include <algorithm>
#include <filesystem>

namespace Engine
{
    EXPORT_RENDERER TextureArrayData* TextureArrayLoad(const char* textureName, char** paths, unsigned int count)
    {
        TextureArrayData* data = Allocator::Allocate<TextureArrayData>();

        (data)->name = strdup(textureName);

        for (unsigned int i = 0; i < count; i++)
        {
            auto file = paths[i];
            if (!std::filesystem::exists(file))
            {
                LOG_ERROR("%s does not exist!\n", file);
                return nullptr;
            }
        }

        glGenTextures(1, &data->id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, data->id);
        bool first = true;
        for (unsigned int textureIndex = 0; textureIndex < count; textureIndex++)
        {
            auto file = paths[textureIndex];
            stbi_uc* image_data = nullptr;
            {
                image_data = stbi_load(file, (int*) &data->width, (int*) &data->height, (int*) &data->channels, 0);
            }
            if (nullptr == image_data) { LOG_ERROR("Can not load %s\n", file); }
            else { LOG_INFO("Loaded %s\n", file); }

            GLenum internalFormat = 0, dataFormat = 0;
            if (data->channels == 4)
            {
                internalFormat = GL_RGBA;
                dataFormat = GL_RGBA;
            }
            else if (data->channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            else if (data->channels == 2)
            {
                internalFormat = GL_RG;
                dataFormat = GL_RG;
            }
            else if (data->channels == 1)
            {
                internalFormat = GL_RED;
                dataFormat = GL_RED;
            }

            if (first)
            {
                glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, data->width, data->height, count, 0, dataFormat,
                             GL_UNSIGNED_BYTE, nullptr);
                first = false;
            }

            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textureIndex, data->width, data->height, 1, dataFormat,
                            GL_UNSIGNED_BYTE, image_data);
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

            stbi_image_free(image_data);
        }
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

        return data;
    }

    EXPORT_RENDERER void TextureArrayBind(TextureArrayData* data, uint32_t slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D_ARRAY, data->id);
    }

    EXPORT_RENDERER void TextureArrayDestroy(TextureArrayData** data)
    {
        if (*data)
        {
            glDeleteTextures(1, &(*data)->id);
            if ((*data)->name && strlen((*data)->name) > 0) { free((*data)->name); }
            Allocator::Deallocate(*data);
            *data = nullptr;
        }
    }

    EXPORT_RENDERER char* TextureArrayGetName(TextureArrayData* data) { return data->name; }

    EXPORT_RENDERER uint32_t TextureArrayGetID(TextureArrayData* data) { return data->id; };

    EXPORT_RENDERER uint32_t TextureArrayGetWidth(TextureArrayData* data) { return data->width; }

    EXPORT_RENDERER uint32_t TextureArrayGetHeight(TextureArrayData* data) { return data->height; }

    EXPORT_RENDERER uint32_t TextureArrayGetChannels(TextureArrayData* data) { return data->channels; }

    EXPORT_RENDERER uint32_t TextureArrayGetCount(TextureArrayData* data) { return data->count; }
}// namespace Engine

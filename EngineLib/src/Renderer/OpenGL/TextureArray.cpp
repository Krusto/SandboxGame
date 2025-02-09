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
    EXPORT_RENDERER void TextureArrayLoad(void** data, const char* textureName, char** paths, unsigned int count)
    {
        if (*data == nullptr) { *data = Allocator::Allocate<TextureArrayData>(); }
        TextureArrayData* m_Data = (TextureArrayData*) *data;

        m_Data->name = strdup(textureName);

        for (unsigned int i = 0; i < count; i++)
        {
            auto file = paths[i];
            if (!std::filesystem::exists(file))
            {
                LOG_ERROR("%s does not exist!\n", file);
                return;
            }
        }

        glGenTextures(1, &m_Data->id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_Data->id);
        bool first = true;
        for (unsigned int textureIndex = 0; textureIndex < count; textureIndex++)
        {
            auto file = paths[textureIndex];
            stbi_uc* image_data = nullptr;
            {
                image_data =
                        stbi_load(file, (int*) &m_Data->width, (int*) &m_Data->height, (int*) &m_Data->channels, 0);
            }
            if (nullptr == image_data) { LOG_ERROR("Can not load %s\n", file); }
            else { LOG_INFO("Loaded %s\n", file); }

            GLenum internalFormat = 0, dataFormat = 0;
            if (m_Data->channels == 4)
            {
                internalFormat = GL_RGBA;
                dataFormat = GL_RGBA;
            }
            else if (m_Data->channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }
            else if (m_Data->channels == 2)
            {
                internalFormat = GL_RG;
                dataFormat = GL_RG;
            }
            else if (m_Data->channels == 1)
            {
                internalFormat = GL_RED;
                dataFormat = GL_RED;
            }

            if (first)
            {
                glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, internalFormat, m_Data->width, m_Data->height, count, 0,
                             dataFormat, GL_UNSIGNED_BYTE, nullptr);
                first = false;
            }

            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textureIndex, m_Data->width, m_Data->height, 1, dataFormat,
                            GL_UNSIGNED_BYTE, image_data);
            glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

            stbi_image_free(image_data);
        }
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    EXPORT_RENDERER void TextureArrayBind(void* data, uint32_t slot)
    {
        TextureArrayData* m_Data = (TextureArrayData*) data;
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_Data->id);
    }

    EXPORT_RENDERER void TextureArrayDestroy(void** data)
    {
        if (*data)
        {
            TextureArrayData* m_Data = (TextureArrayData*) data;
            glDeleteTextures(1, &m_Data->id);
            if (m_Data->name) { free(m_Data->name); }
            Allocator::Deallocate(m_Data);
            *data = nullptr;
        }
    }

    EXPORT_RENDERER char* TextureArrayGetName(void* data)
    {
        TextureArrayData* m_Data = (TextureArrayData*) data;
        return m_Data->name;
    }

    EXPORT_RENDERER uint32_t TextureArrayGetID(void* data)
    {
        TextureArrayData* m_Data = (TextureArrayData*) data;
        return m_Data->id;
    };

    EXPORT_RENDERER uint32_t TextureArrayGetWidth(void* data)
    {
        TextureArrayData* m_Data = (TextureArrayData*) data;
        return m_Data->width;
    }

    EXPORT_RENDERER uint32_t TextureArrayGetHeight(void* data)
    {
        TextureArrayData* m_Data = (TextureArrayData*) data;
        return m_Data->height;
    }

    EXPORT_RENDERER uint32_t TextureArrayGetChannels(void* data)
    {
        TextureArrayData* m_Data = (TextureArrayData*) data;
        return m_Data->channels;
    }

    EXPORT_RENDERER uint32_t TextureArrayGetCount(void* data)
    {
        TextureArrayData* m_Data = (TextureArrayData*) data;
        return m_Data->count;
    }
}// namespace Engine

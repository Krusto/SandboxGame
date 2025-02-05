#include "TextureArray.hpp"
#include <Renderer/Shared/function_pointers.h>

namespace Engine
{
    TextureArray TextureArray::Create() { return TextureArray(); }

    void TextureArray::Load(std::string_view arrayName, std::initializer_list<std::string> Path)
    {
        char** paths = new char*[Path.size() + 1];
        int i = 0;
        for (auto& path: Path) { paths[i++] = strdup(path.c_str()); }
        paths[i] = nullptr;
        TextureArrayLoad((void**) &m_Data, arrayName.data(), paths, (unsigned int) Path.size());
        for (i = 0; i < Path.size(); i++) { delete[] paths[i]; }
        delete[] paths;
    }

    void TextureArray::Load(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path)
    {
        char** paths = new char*[Path.size() + 1];
        for (auto& path: Path) { paths[path.first] = strdup(path.second.c_str()); }
        TextureArrayLoad((void**) &m_Data, arrayName.data(), paths, (unsigned int) Path.size());
        for (int i = 0; i < Path.size(); i++) { delete[] paths[i]; }
        delete[] paths;
    }

    void TextureArray::Bind(uint32_t slot) const { TextureArrayBind((void*) m_Data, slot); }

    void TextureArray::Destroy() { TextureArrayDestroy((void**) &m_Data); }

    std::string_view TextureArray::GetName() const
    {
        return std::string_view(TextureArrayGetName((void*) m_Data), strlen(TextureArrayGetName((void*) m_Data)));
    }

    uint32_t TextureArray::ID() const { return TextureArrayGetID((void*) m_Data); }

    uint32_t TextureArray::Width() const { return TextureArrayGetWidth((void*) m_Data); }

    uint32_t TextureArray::Height() const { return TextureArrayGetHeight((void*) m_Data); }

    uint32_t TextureArray::Channels() const { return TextureArrayGetChannels((void*) m_Data); }
}// namespace Engine

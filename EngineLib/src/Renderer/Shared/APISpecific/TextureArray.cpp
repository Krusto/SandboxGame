#include "TextureArray.hpp"
#include <Renderer/Renderer.hpp>

namespace Engine
{
    TextureArray::TextureArray(std::string_view arrayName, std::initializer_list<std::string> Path)
    {
        Load(arrayName, Path);
    }

    TextureArray::TextureArray(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path)
    {
        Load(arrayName, Path);
    }

    TextureArray TextureArray::Create(std::string_view arrayName, std::initializer_list<std::string> Path)
    {
        return TextureArray(arrayName, Path);
    }

    TextureArray TextureArray::Create(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path)
    {
        return TextureArray(arrayName, Path);
    }

    void TextureArray::Load(std::string_view arrayName, std::initializer_list<std::string> Path)
    {
        char** paths = new char*[Path.size() + 1];
        int i = 0;
        for (auto& path: Path) { paths[i++] = strdup(path.c_str()); }
        m_Data = Renderer::GetInstance()->TextureArrayLoad(arrayName.data(), paths, (unsigned int) Path.size());
        for (i = 0; i < Path.size(); i++) { delete[] paths[i]; }
        delete[] paths;
    }

    void TextureArray::Load(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path)
    {
        char** paths = new char*[Path.size() + 1];
        for (auto& path: Path) { paths[path.first] = strdup(path.second.c_str()); }
        m_Data = Renderer::GetInstance()->TextureArrayLoad(arrayName.data(), paths, (unsigned int) Path.size());
        for (int i = 0; i < Path.size(); i++) { delete[] paths[i]; }
        delete[] paths;
    }

    void TextureArray::Bind(uint32_t slot) const { Renderer::GetInstance()->TextureArrayBind(m_Data, slot); }

    void TextureArray::Destroy() { Renderer::GetInstance()->TextureArrayDestroy(&m_Data); }

    std::string_view TextureArray::GetName() const
    {
        return std::string_view(Renderer::GetInstance()->TextureArrayGetName(m_Data),
                                strlen(Renderer::GetInstance()->TextureArrayGetName(m_Data)));
    }

    uint32_t TextureArray::ID() const { return Renderer::GetInstance()->TextureArrayGetID(m_Data); }

    uint32_t TextureArray::Width() const { return Renderer::GetInstance()->TextureArrayGetWidth(m_Data); }

    uint32_t TextureArray::Height() const { return Renderer::GetInstance()->TextureArrayGetHeight(m_Data); }

    uint32_t TextureArray::Channels() const { return Renderer::GetInstance()->TextureArrayGetChannels(m_Data); }
}// namespace Engine

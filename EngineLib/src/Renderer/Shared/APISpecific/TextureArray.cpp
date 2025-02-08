#include "TextureArray.hpp"
#include <Renderer/Renderer.hpp>

namespace Engine
{
    TextureArray TextureArray::Create() { return TextureArray(); }

    void TextureArray::Load(std::string_view arrayName, std::initializer_list<std::string> Path)
    {
        char** paths = new char*[Path.size() + 1];
        int i = 0;
        for (auto& path: Path) { paths[i++] = strdup(path.c_str()); }
        paths[i] = nullptr;
        Renderer::GetInstance()->TextureArrayLoad((void**) &m_Data, arrayName.data(), paths,
                                                  (unsigned int) Path.size());
        for (i = 0; i < Path.size(); i++) { delete[] paths[i]; }
        delete[] paths;
    }

    void TextureArray::Load(std::string_view arrayName, const std::unordered_map<uint32_t, std::string>& Path)
    {
        char** paths = new char*[Path.size() + 1];
        for (auto& path: Path) { paths[path.first] = strdup(path.second.c_str()); }
        Renderer::GetInstance()->TextureArrayLoad((void**) &m_Data, arrayName.data(), paths,
                                                  (unsigned int) Path.size());
        for (int i = 0; i < Path.size(); i++) { delete[] paths[i]; }
        delete[] paths;
    }

    void TextureArray::Bind(uint32_t slot) const { Renderer::GetInstance()->TextureArrayBind((void*) m_Data, slot); }

    void TextureArray::Destroy() { Renderer::GetInstance()->TextureArrayDestroy((void**) &m_Data); }

    std::string_view TextureArray::GetName() const
    {
        return std::string_view(Renderer::GetInstance()->TextureArrayGetName((void*) m_Data),
                                strlen(Renderer::GetInstance()->TextureArrayGetName((void*) m_Data)));
    }

    uint32_t TextureArray::ID() const { return Renderer::GetInstance()->TextureArrayGetID((void*) m_Data); }

    uint32_t TextureArray::Width() const { return Renderer::GetInstance()->TextureArrayGetWidth((void*) m_Data); }

    uint32_t TextureArray::Height() const { return Renderer::GetInstance()->TextureArrayGetHeight((void*) m_Data); }

    uint32_t TextureArray::Channels() const { return Renderer::GetInstance()->TextureArrayGetChannels((void*) m_Data); }
}// namespace Engine

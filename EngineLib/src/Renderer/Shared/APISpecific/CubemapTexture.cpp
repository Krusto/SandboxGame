#include "CubemapTexture.hpp"
#include <Renderer/Shared/function_pointers.h>

namespace Engine
{
    CubemapTexture CubemapTexture::Create(std::string_view cubemapName,
                                          const std::unordered_map<CubemapTextureFace, std::string>& Path)
    {
        CubemapTexture ptr;
        ptr.Load(cubemapName, Path);
        return ptr;
    }

    void CubemapTexture::Load(std::string_view cubemapName,
                              const std::unordered_map<CubemapTextureFace, std::string>& Path)
    {
        CubemapTextureLoad((void**) & m_Data, (void*)&cubemapName, (void*)&Path);
    }

    void CubemapTexture::Destroy() { CubemapTextureDestroy((void**) & m_Data); }

    const CubemapTextureSpec* CubemapTexture::GetSpec() const
    { 
        return (CubemapTextureSpec*)CubemapTextureGetSpec((void*) m_Data);
    }

    std::string_view CubemapTexture::GetName() const
    {
        return ((CubemapTextureSpec*) CubemapTextureGetSpec((void*) m_Data))->Name;
    }

    uint32_t CubemapTexture::ID() const { return (uint32_t) CubemapTextureGetID((void*) m_Data); }

    uint32_t CubemapTexture::Width() const
    {
        return ((CubemapTextureSpec*) CubemapTextureGetSpec((void*) m_Data))->Width;
    }

    uint32_t CubemapTexture::Height() const
    {
        return ((CubemapTextureSpec*) CubemapTextureGetSpec((void*) m_Data))->Height;
    }

    uint32_t CubemapTexture::Channels() const { return ((CubemapTextureSpec*) CubemapTextureGetSpec((void*) m_Data))->Channels; }

    void CubemapTexture::Bind(uint32_t slot) const { CubemapTextureBind((void*) m_Data, slot); }

}// namespace Engine
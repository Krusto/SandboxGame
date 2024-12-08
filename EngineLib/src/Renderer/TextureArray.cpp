#include "TextureArray.hpp"
#include <Core/Allocator.hpp>
#include "OpenGL/OpenGLTextureArray.hpp"

namespace Engine
{
    TextureArray* TextureArray::Create() { return (TextureArray*) Allocator::Allocate<OpenGLTextureArray>(); }
}// namespace Engine

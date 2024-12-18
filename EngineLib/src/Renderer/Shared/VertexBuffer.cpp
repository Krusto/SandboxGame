#include "VertexBuffer.hpp"

namespace Engine
{

    VertexBuffer VertexBuffer::Create(VertexArray* va, const VertexLayout& layout, float* data, uint32_t length)
    {
        VertexBuffer ptr;
        ptr.Init(va, layout, data, length);
        return ptr;
    }

}// namespace Engine
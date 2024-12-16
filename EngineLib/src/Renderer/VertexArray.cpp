#include "VertexArray.hpp"

namespace Engine
{

    VertexArray VertexArray::Create(uint32_t indexCount)
    {
        VertexArray ptr;
        ptr.Init(indexCount);
        return ptr;
    }

}// namespace Engine
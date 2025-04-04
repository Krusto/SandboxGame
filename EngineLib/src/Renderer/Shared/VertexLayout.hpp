//
// Created by Stoyanov, Krusto (K.S.) on 6/23/2022.
//
#ifndef ENGINE_VERTEXLAYOUT_H
#define ENGINE_VERTEXLAYOUT_H

/*---------------------------------------------------------------------------------------------------
Includes
---------------------------------------------------------------------------------------------------*/
#include "ShaderUniform.h"

#ifdef __cplusplus
namespace Engine
{
#endif

//----------------------------------------------------------------------------------------------------
//Macro Definitions
//----------------------------------------------------------------------------------------------------
#define VertexLayoutGetStride(layout) ((VertexLayout*) layout)->stride
#define VERTEX_MAX_ATTRIBUTES 16

    //---------------------------------------------------------------------------------------------------
    //Structures
    //---------------------------------------------------------------------------------------------------
    struct VertexAttribute {
        const char* name;
        uint8_t type;
        uint32_t offset;
    };

    struct VertexAttributeList {
        VertexAttribute attributes[VERTEX_MAX_ATTRIBUTES];
        uint32_t count;
    };

    struct VertexLayout {
        VertexAttributeList attributes;
        std::size_t stride;
    };

    //---------------------------------------------------------------------------------------------------
    //Functions
    //---------------------------------------------------------------------------------------------------
    inline static VertexLayout CreateVertexLayout(VertexAttributeList attributes)
    {
        VertexLayout layout;
        layout.attributes = attributes;
        layout.stride = 0;
        for (size_t i = 0; i < attributes.count && attributes.attributes[i].name != nullptr; i++)
        {
            layout.attributes.attributes[i].offset = layout.stride;
            layout.stride += ShaderDataTypeGetSize((ShaderUniformType) attributes.attributes[i].type);
        }
        return layout;
    }


#ifdef __cplusplus
}// namespace Engine
#endif
#endif
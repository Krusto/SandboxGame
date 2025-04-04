#ifndef CSTRING_VIEW_H
#define CSTRING_VIEW_H
#include <Core/CTypes.h>
#ifdef __cplusplus
namespace Engine
{
#endif

    typedef struct {
        const char* data;
        size_t length;
    } CStringView;


#ifdef __cplusplus
}
#endif
#endif

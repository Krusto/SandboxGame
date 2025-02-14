#ifndef CSTRING_VIEW_H
#define CSTRING_VIEW_H

#ifdef __cplusplus
namespace Engine
{
#endif

    typedef struct CStringView {
        const char* data;
        size_t length;
    } CStringView;


#ifdef __cplusplus
}
#endif
#endif

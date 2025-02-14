#ifndef CPAIR_H
#define CPAIR_H

#ifdef __cplusplus
namespace Engine
{
#endif

#include "CStringView.h"

#define DeclareCPair(first_type, second_type, pair_type_name)                                                          \
    typedef struct {                                                                                                   \
        first_type first;                                                                                              \
        second_type second;                                                                                            \
    } pair_type_name;

    DeclareCPair(void*, void*, CPair_void_void);
    DeclareCPair(CStringView, CStringView, CPairStrView);

#ifdef __cplusplus
}
#endif
#endif

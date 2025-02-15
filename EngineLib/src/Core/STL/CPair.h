#ifndef CPAIR_H
#define CPAIR_H

#include "CStringView.h"

#ifdef __cplusplus
namespace Engine
{
#endif


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

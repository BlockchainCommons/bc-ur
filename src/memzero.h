#ifndef BC_UR_MEMZERO_H
#define BC_UR_MEMZERO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void memzero(void* const pnt, const size_t len);

#ifdef __cplusplus
}
#endif

#endif // BC_UR_MEMZERO_H

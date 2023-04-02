#ifndef __ONIX_DEBUG_H__
#define __ONIX_DEBUG_H__

#include <onix/types.h>

void debugk(int8 *file, u32 line, const int8 * fmt, ...);
#define BMB asm volatile("xchgw %bx, %bx")
#define DEBUGK(fmt, args...) debugk(__BASE_FILE__, __LINE__, fmt, ##args)

#endif
#ifndef __ONIX_DEBUG_H__
#define __ONIX_DEBUG_H__

#include <onix/types.h>

void debugk(int8 *file, u32 line, const int8 * fmt, ...);

#define DEBUGK(fmt, args...) debugk(__BASE_FILE__, __LINE__, fmt, ##args)

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define BMB LOGK("BMB\n"); asm volatile("xchgw %bx, %bx")

#endif
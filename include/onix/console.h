#ifndef __ONIX_CONSOLE_H__
#define __ONIX_CONSOLE_H__

#include <onix/types.h>

struct point {
    u32 x;
    u32 y;
};


void console_init();
void console_clear();
void console_write(int8 *buf, u32 count);






#endif
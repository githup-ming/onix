#ifndef __ONIX_STDIO_H__
#define __ONIX_STDIO_H__

#include <onix/types.h>


int vsprintf(int8 *buf, const int8 * fmt, va_list args);
int sprintf(int8 *buf, const int8 * fmt, ...);


#endif
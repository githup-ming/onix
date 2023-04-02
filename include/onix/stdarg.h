#ifndef __ONIX_STDARG_H__
#define __ONIX_STDARG_H__

#include <onix/types.h>


//ap 指向v的下一个内存地址
#define va_start(ap, v) (ap = (va_list)&v + sizeof(int8 *))
//ap指向的内存地址上的内容，并把ap指向下一个地址
#define va_arg(ap, t) (*(t *)((ap += sizeof(int8 *)) - sizeof(int8 *)))
//把ap清空
#define va_end(ap) (ap = (va_list)NULL)

#endif
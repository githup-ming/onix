#include <onix/debug.h>
#include <onix/stdarg.h>
#include <onix/stdio.h>
#include <onix/printk.h>


static int8 buf[1024];

void debugk(int8 *file, u32 line, const int8 * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    printk("[%s %d] %s", file, line, buf);
}
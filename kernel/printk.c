#include <onix/printk.h>
#include <onix/stdarg.h>
#include <onix/stdio.h>
#include <onix/console.h>

static int8 buf[1024];

int printk(const int8 *fmt, ...)
{
    va_list args;
    int i;

    va_start(args, fmt);

    i = vsprintf(buf, fmt, args);

    va_end(args);

    console_write(buf, i);

    return i;
}


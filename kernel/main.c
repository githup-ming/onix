#include <onix/onix.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/console.h>
#include <onix/stdarg.h>
#include <onix/printk.h>
#include <onix/assert.h>
#include <onix/debug.h>


void kernel_init()
{

    console_init();

    DEBUGK("43241\n");

    BMB;

    DEBUGK("abcd\n");
    

    return;

}
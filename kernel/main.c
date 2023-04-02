#include <onix/onix.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/console.h>

char message[] = "hello on6666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666ix!!!\n";//data

void kernel_init()
{

    console_init();
    u32 count = 80;
    while (true)
    {
        console_write(message, sizeof(message) - 1);
    }

    return;

}
#include <onix/onix.h>
#include <onix/io.h>
#include <onix/string.h>
#include <onix/console.h>
#include <onix/stdarg.h>
#include <onix/printk.h>
#include <onix/assert.h>
#include <onix/debug.h>
#include <onix/global.h>
#include <onix/task.h>
#include <onix/interrupt.h>
#include <onix/stdlib.h>
#include <onix/clock.h>
#include <onix/time.h>
#include <onix/rtc.h>
#include <onix/memory.h>
#include <onix/bitmap.h>
#include <onix/gate.h>
#include <onix/list.h>

void kernel_init()
{
    memory_map_init();
    mapping_init();
    interrupt_init();

    clock_init();
    // // time_init();
    // // rtc_init();
    task_init();
    syscall_init();
    // // memory_test();
    // // bitmap_tests();

    // list_test();
    set_interrupt_state(true);


    LOGK("kernel_init end\n");
    // hang();
}
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


void intr_test()
{
    bool intr = interrupt_disable();


    set_interrupt_state(intr);
}

void kernel_init()
{
    memory_map_init();
    mapping_init();
    interrupt_init();
    // task_init();
    // clock_init();
    // time_init();
    // rtc_init();

    // memory_test();
    // bitmap_tests();
    // BMB;
    bool intr = interrupt_disable();
    set_interrupt_state(true);

    LOGK("%d\n",intr);
    LOGK("%d\n",get_interrupt_state());

    BMB;

    intr = interrupt_disable();
    LOGK("%d\n",intr);
    LOGK("%d\n",get_interrupt_state());




    // asm volatile("sti");//开中断
    DEBUGK("kernel_init end\n");
    hang();
}
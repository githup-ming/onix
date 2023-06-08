#include <onix/thread.h>
#include <onix/syscall.h>
#include <onix/debug.h>
#include <onix/interrupt.h>



// 空闲任务
void idle_thread()
{
    set_interrupt_state(true);
    // u32 counter = 0;
    while (true)
    {
        // LOGK("idle task ... %d\n", counter++);
        asm volatile(
            "sti\n" // 开中断
            "hlt\n" // 关闭cpu 进入暂停状态，等待外中断到来
        );
        yield(); // 放弃执行权，调度执行其他程序
    }

}
#include <onix/mutex.h>

spinlock_t lock;


void init_thread()
{
    spin_init(&lock);
    set_interrupt_state(true);

    while (true)
    {
        // spin_lock(&lock);
        LOGK("init task ...\n");
        // spin_unlock(&lock);
        // sleep(100);
    }

}

void test_thread()
{
    set_interrupt_state(true);

    while (true)
    {
        // spin_lock(&lock);
        LOGK("test_thread task ...\n");
        // spin_unlock(&lock);
        // sleep(3000);
    }

}
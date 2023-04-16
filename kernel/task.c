#include <onix/task.h>
#include <onix/printk.h>
#include <onix/debug.h>


#define PAGE_SIZE 0X1000   //一页内存大小4k

task_t *a = (task_t *)0x1000;
task_t *b = (task_t *)0x2000;


extern void task_switch(task_t *next);

task_t *running_task()
{
    asm volatile(
        "movl %esp, %eax\n"
        "andl $0xfffff000, %eax\n"
    );
}

void schedule()
{
    task_t *current = running_task();
    task_t *next = current == a ? b : a;
    task_switch(next);
}

u32 _ofp thread_a()
{
    asm volatile("sti\n");
    while (true)
    {
        printk("a");
    }
}

u32 _ofp thread_b()
{
    asm volatile("sti\n");
    while (true)
    {
        printk("b");
    }
}

void task_creat(task_t *task, target_t targer)
{
    u32 stack = (u32)task + PAGE_SIZE;

    stack -= sizeof(task_frame_t);
    task_frame_t *frame = (task_frame_t *)stack;
    frame->edi = 0x11111111;
    frame->esi = 0x22222222;
    frame->ebx = 0x33333333;
    frame->ebp = 0x44444444;
    frame->eip = (void *)targer;

    task->stack = (u32 *)stack;//任务块
}

void task_init()
{
    task_creat(a, thread_a);
    task_creat(b, thread_b);
    schedule();
}
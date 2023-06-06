#include <onix/task.h>
#include <onix/printk.h>
#include <onix/debug.h>
#include <onix/memory.h>
#include <onix/assert.h>
#include <onix/interrupt.h>
#include <onix/string.h>
#include <onix/bitmap.h>
#include <onix/syscall.h>
#include <onix/list.h>
#include <onix/thread.h>

extern u32 volatile jiffies;//全局时间片
extern u32 jiffy;//1个时间片的毫秒值
extern bitmap_t kernel_map;
extern void task_switch(task_t *next);

#define NR_TASKS 64
static task_t *task_table[NR_TASKS];// 任务表
static list_t block_list;// 任务默认阻塞链表
static list_t sleep_list;// 任务睡眠链表
static task_t *idle_task;// 空闲任务

//从task_table中获得一个空闲任务
static task_t *get_free_task()
{
    for (size_t i = 0; i < NR_TASKS; i++) {
        if (task_table[i] == NULL) {
            task_table[i] = (task_t *)alloc_kpage(1);// .todo. free_kpage
            return task_table[i];
        }
    }
    panic("no more tasks");
    
}

// 从任务数组中查找某种状态的任务，自己除外
static task_t *task_search(task_state_t state)
{
    assert(!get_interrupt_state());
    task_t *task = NULL;

    task_t *current = running_task();

    for (size_t i = 0; i < NR_TASKS; i++) {
        task_t *ptr = task_table[i];
        if (ptr == NULL) {
            continue;
        }
        if (ptr->state != state) {
            continue;
        }
        if (current == ptr) {
            continue;
        }
        
        if ((task == NULL) || (task->ticks < ptr->ticks) || (ptr->jiffies < task->jiffies)) {
            task = ptr;
        }
    }

    if (task == NULL && state == TASK_READY) {
        task = idle_task;
    }
    
    return task;
}

task_t *running_task()
{
    asm volatile(
        "movl %esp, %eax\n"
        "andl $0xfffff000, %eax\n"
    );
}

void schedule()
{
    assert(!get_interrupt_state());//不可中断

    task_t *current = running_task();
    task_t *next = task_search(TASK_READY);

    assert(next != NULL);
    assert(next->magic == ONIX_MAGIC);

    if (current->state == TASK_RUNNING) {
        current->state = TASK_READY;
    }
    
    next->state = TASK_RUNNING;
    if (next == current) {
        return;
    }
    
    task_switch(next);
}

static task_t *task_creat(target_t targer, const int8 *name, u32 pricrity, u32 uid)
{
    task_t *task = get_free_task();
    memset(task, 0, PAGE_SIZE);

    u32 stack = (u32)task + PAGE_SIZE;

    stack -= sizeof(task_frame_t);
    task_frame_t *frame = (task_frame_t *)stack;
    frame->edi = 0x11111111;
    frame->esi = 0x22222222;
    frame->ebx = 0x33333333;
    frame->ebp = 0x44444444;
    frame->eip = (void *)targer;

    strcpy((char *)task->name, name);

    task->stack = (u32 *)stack;//任务块
    task->priority = pricrity;
    task->ticks = task->priority;
    task->jiffies = 0;
    task->state = TASK_READY;
    task->uid = uid;
    task->pde = KERNEL_PAGE_DIR;
    task->vmap = &kernel_map;
    task->magic = ONIX_MAGIC;

    return task;
}

static void task_setup()
{
    task_t *task = running_task();
    task->magic = ONIX_MAGIC;
    task->ticks = 1;

    memset(task_table, 0, sizeof(task_table));
}

// 任务阻塞
void task_block(task_t *task, list_t *blist, task_state_t state)
{
    assert(task->node.next == NULL);
    assert(task->node.prev == NULL);

    if (blist == NULL) {
        blist = &block_list;
    }
    list_push(blist, &task->node);

    assert(state != TASK_READY && state != TASK_RUNNING);

    task->state = state;

    task_t *current = running_task();
    if (current == task) {
        schedule();
    }
    
}

// 任务解除阻塞
void task_unblock(task_t *task)
{
    assert(!get_interrupt_state());

    list_remove(&task->node);

    assert(task->node.next == NULL);
    assert(task->node.prev == NULL);

    task->state = TASK_READY;
}
// 任务进入睡眠
void task_sleep(u32 ms)
{
    assert(!get_interrupt_state());

    u32 ticks = ms / jiffy;//需要睡眠的时间片
    ticks = ticks > 0 ? ticks: 1;

    //记录目标时间片，在那个时刻需要唤醒
    task_t *current = running_task();
    current->ticks = jiffies + ticks;

    // 插入到睡眠链表，并按唤醒时间从小到大排序
    list_t *list = &sleep_list;
    list_node_t *anchor = &list->tail;

    for (list_node_t *ptr = list->head.next; ptr != &list->tail; ptr = ptr->next) {
        task_t *task = element_entry(task_t, node, ptr);
        if (task->ticks > current->ticks) {
            anchor = ptr;
            break;
        }
    }
    // 插入链表
    list_insert_before(anchor, &current->node);

    // 阻塞状态是睡眠
    current->state = TASK_SLEEPING;

    // 调度到其他任务
    schedule();
    

}

void task_wakeup()
{
    assert(!get_interrupt_state());

    // 从睡眠链表中找到 ticks 小于 jiffies 的任务 ，恢复执行
    list_t *list = &sleep_list;
    for (list_node_t *ptr = list->head.next; ptr != &list->tail;) {
        task_t *task = element_entry(task_t, node, ptr);
        if (task->ticks > jiffies) {
            break;
        }
        
        ptr = ptr->next;

        task->ticks = 0;
        task_unblock(task);
    }
}

// 任务切换
void task_yield()
{
    schedule();
}

// task 初始化
void task_init()
{
    list_init(&block_list);
    list_init(&sleep_list);

    task_setup();

    idle_task = task_creat(idle_thread, "idle", 1, KERNEL_USER);
    task_creat(init_thread, "init", 5, NORMAL_USER);
    // task_creat(test_thread, "test", 5, KERNEL_USER);
    
}
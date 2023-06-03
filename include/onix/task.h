#ifndef __ONIX_TASK_H__
#define __ONIX_TASK_H__

#include <onix/types.h>
#include <onix/bitmap.h>

#define KERNEL_USER 0
#define NORMAL_USER 1

#define TASK_NAME_LEN 16



typedef u32 target_t();

typedef enum task_state
{
    TASK_INIT, //初始化
    TASK_RUNNING, //执行
    TASK_READY, //就绪
    TASK_BLOCKED, // 阻塞
    TASK_SLEEPING, // 睡眠
    TASK_WAITING, // 等待
    TASK_DIED, // 死亡
} task_state_t;

typedef struct task_t
{
    u32 *stack; //内核栈
    task_state_t state; // 任务状态
    u32 priority; // 任务优先级
    u32 ticks; // 剩余时间片
    u32 jiffies; // 上次执行时全局的时间片
    u8 name[TASK_NAME_LEN]; //任务名
    u32 uid; // 用户id
    u32 pde; // 页目录地址
    bitmap_t *vmap; // 进程虚拟内存位图
    u32 magic; // 内核魔数，用于检测内存溢出
} task_t;

typedef struct task_frame_t
{
    u32 edi;
    u32 esi;
    u32 ebx;
    u32 ebp;
    void (*eip)(void);
} task_frame_t;


void task_init();

task_t *running_task();
void schedule();

#endif
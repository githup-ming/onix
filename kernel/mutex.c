#include <onix/mutex.h>
#include <onix/interrupt.h>
#include <onix/task.h>
#include <onix/assert.h>

//初始化互斥锁
void mutex_init(mutex_t *mutex)
{
    mutex->value = false;// 初始化没人持有
    list_init(&mutex->waiters);
}

//尝试持有互斥量
void mutex_lock(mutex_t *mutex)
{
    // 关中断，保证原子操作
    bool intr = interrupt_disable();

    task_t *current = running_task();
    while (mutex->value == true) {
        // 若value 为true 表示已经被别人持有
        // 则将当前任务加入到等待队列
        task_block(current, &mutex->waiters, TASK_BLOCKED);
    }
    // 无人持有
    assert(mutex->value == false);

    // 持有
    mutex->value++;
    assert(mutex->value == true);

    // 恢复中断
    set_interrupt_state(intr);
    
}

//释放互斥量
void mutex_unlock(mutex_t *mutex)
{
    // 关中断，保证原子操作
    bool intr = interrupt_disable();

    // 已持有互斥量
    assert(mutex->value == true);

    // 取消持有
    mutex->value--;
    assert(mutex->value == false);

    // 如果等待队列不为空则恢复执行
    if (!list_empty(&mutex->waiters)) {
        task_t *task = element_entry(task_t, node, mutex->waiters.tail.prev);
        assert(task->magic == ONIX_MAGIC);
        task_unblock(task);
        // 保证新进程能获得互斥量，不可能饿死
        task_yield();
    }

    // 恢复中断
    set_interrupt_state(intr);
}



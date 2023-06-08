#ifndef __ONIX_MUTEX_H__
#define __ONIX_MUTEX_H__

#include <onix/types.h>
#include <onix/list.h>

typedef struct mutex
{
    bool value;//信号量
    list_t waiters;//等待队列
} mutex_t;

void mutex_init(mutex_t *mutex);//初始化互斥锁
void mutex_lock(mutex_t *mutex);//尝试持有互斥量
void mutex_unlock(mutex_t *mutex);//释放互斥量

#endif
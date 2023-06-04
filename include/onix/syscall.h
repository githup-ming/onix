#ifndef __ONIX_SYSCALL_H__
#define __ONIX_SYSCALL_H__

#include <onix/types.h>


typedef enum syscall
{
    SYS_NR_TEST,
    SYS_NR_YIELD,
} syscall_t;


u32 test();
void yield();



#endif
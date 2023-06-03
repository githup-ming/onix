#ifndef __ONIX_GATE_H__
#define __ONIX_GATE_H__

#include <onix/types.h>

void syscall_init();
void syscall_check(u32 nr);

#endif
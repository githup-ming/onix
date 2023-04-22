#ifndef __ONIX_MEMORY_H__
#define __ONIX_MEMORY_H__

#include <onix/types.h>

#define MEMORY_BASE 0x100000 // 1M，可用内存开始的位置
#define PAGE_SIZE 0x1000     // 一页的大小 4K

void memory_map_init();
void memory_test();

#endif
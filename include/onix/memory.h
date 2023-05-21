#ifndef __ONIX_MEMORY_H__
#define __ONIX_MEMORY_H__

#include <onix/types.h>

#define MEMORY_BASE 0x100000 // 1M，可用内存开始的位置
#define PAGE_SIZE 0x1000     // 一页的大小 4K

void memory_map_init();
void memory_test();

typedef struct page_entry_t
{
    u8 present : 1; //在内存中 bit0
    u8 write : 1;//0 只读，1 可读可写 bit1
    u8 user : 1; //1 所有人， 0 超级用户 DPL < 3 bit2
    u8 pwt : 1; //page write through 1 只写模式， 0 会写模式 bit3
    u8 pcd : 1; //page cache disable 禁止该页缓存 bit4
    u8 accessed : 1;//被访问过，用于统计使用频率 bit5
    u8 dirty : 1;//脏页，表示该页缓存别写过bit6
    u8 pat : 1;//page attribute table y页大小 bit7
    u8 global : 1;//全局，所有进程都用到了，该页不刷新缓存 bit8
    u8 ignored : 3;//保留 bit9 - bit11
    u32 index : 20;//页索引 bit12 - bit31
} _packed page_entry_t;

// 得到cr3 寄存器
u32 get_cr3();

// 设置cr3的地址，参数是页目录的地址
void set_cr3(u32 pde);
void mapping_init();

// 分配count 个连续的内核页
u32 alloc_kpage(u32 count);

// 释放 count 个连续的内核页
void free_kpage(u32 vaddr, u32 count);


#endif
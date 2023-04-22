#include <onix/memory.h>
#include <onix/debug.h>
#include <onix/assert.h>



#define LOGK(fmt, args...) DEBUGK(fmt, ##args)


#define ZONE_VALID 1  //ARDS 可用内存区域
#define ZONE_RESERVED 2 //ARDS 不可用内存区域



#define IDX(addr) ((u32)addr >> 12) //获取页索引


typedef struct ards_t
{
    u64 base; //内存基地址
    u64 size; //内存长度
    u32 type; //类型
}_packed ards_t;

u32 memory_base = 0; //可用内存的基地址，应该等于1M
u32 memory_size = 0; //可用内存大小
u32 total_pages = 0; //所有内存页数
u32 free_pages = 0; //空闲内存页数

#define used_pages (total_pages - free_pages) //已用页数

void memory_init(u32 magic, u32 addr)
{
    u32 count;
    ards_t *ptr;

    if (magic == ONIX_MAGIC) {
        count = *(u32 *)addr;
        ptr = (ards_t *)(addr + 4);
        for (size_t i = 0; i < count; i++, ptr++)
        {
            LOGK("memory base 0x%p size 0x%p type 0x%p\n",
                 (u32)ptr->base, (u32)ptr->size, (u32)ptr->type);

            if (ptr->type == ZONE_VALID && ptr->size > memory_size) {
                memory_base = (u32)ptr->base;
                memory_size = (u32)ptr->size;
            }
        }
    } else {
        panic("memory init magic unknown 0x%x\n",magic);
    }

    LOGK("ards count %d\n", count);
    LOGK("memory base 0x%p size 0x%p\n", memory_base, memory_size);

    assert(memory_base == MEMORY_BASE); //内存开始的位置1M
    assert((memory_size & 0Xfff) == 0); //按页对齐,一页4k = 0xfff

    total_pages = IDX(memory_size) + IDX(MEMORY_BASE);
    free_pages = IDX(memory_size);

    LOGK("total pages %d free pages %d\n", total_pages, free_pages);

}
















#include <onix/memory.h>
#include <onix/debug.h>
#include <onix/assert.h>
#include <onix/string.h>
#include <onix/stdlib.h>
#include <onix/bitmap.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)


#define ZONE_VALID 1  //ARDS 可用内存区域
#define ZONE_RESERVED 2 //ARDS 不可用内存区域



#define IDX(paddr) ((u32)paddr >> 12) //获取页索引
#define DIDX(vaddr) (((u32)vaddr >> 22) & 0x3ff) //获取页目录索引
#define TIDX(vaddr) (((u32)vaddr >> 12) & 0x3ff) //获取页表索引
#define PAGE(idx) ((u32)idx << 12) //获取索引所在页开始的位置
#define ASSERT_PAGE(addr) assert((addr & 0xfff) == 0) //断言是否是一个页的地址

// 内核页表物理地址,内核所用的内存实际8MB-1MB
static u32 KERNEL_PAGE_TABLE[] = {
    0x2000, //管理 (0 - 0x3FFFFF) 的物理地址，大小4MB
    0x3000, //管理 (0x400000 - 0x7FFFFF) 的物理地址，大小4MB
};

#define KERNEL_MEMORY_SIZE (0x100000 * sizeof(KERNEL_PAGE_TABLE))
#define KERNEL_MAP_BITS 0x4000

bitmap_t kernel_map;// 内核位图
typedef struct ards_t
{
    u64 base; //内存基地址
    u64 size; //内存长度
    u32 type; //类型
}_packed ards_t;

static u32 memory_base = 0; //可用内存的基地址，应该等于1M
static u32 memory_size = 0; //可用内存大小
static u32 total_pages = 0; //所有内存页数
static u32 free_pages = 0; //空闲内存页数

#define used_pages (total_pages - free_pages) //已用页数

void memory_init(u32 magic, u32 addr)
{
    u32 count;
    ards_t *ptr;

    if (magic == ONIX_MAGIC) {
        count = *(u32 *)addr;
        ptr = (ards_t *)(addr + 4);
        for (size_t i = 0; i < count; i++, ptr++) {
            LOGK("memory base 0x%p size 0x%p type %d\n",
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
    LOGK("memory base 0x%p size 0x%p (%dMB)\n", memory_base, memory_size, memory_size/1024/1024);

    assert(memory_base == MEMORY_BASE); //内存开始的位置1M
    assert((memory_size & 0Xfff) == 0); //按页对齐,一页4k = 0xfff

    total_pages = IDX(memory_size) + IDX(MEMORY_BASE);
    free_pages = IDX(memory_size);

    LOGK("total pages %d free pages %d\n", total_pages, free_pages);

    if (memory_size < KERNEL_MEMORY_SIZE)
    {
        panic("system memory is %dMB too small, at least %dMB need \n",
            memory_size / MEMORY_BASE, KERNEL_MEMORY_SIZE / MEMORY_BASE);
    }

}
static u32 start_page = 0; // 可分配物理内存起始位置
static u8 *memory_map;//物理内存数组，用来标记物理内存页是否被使用
static u32 memory_map_pages; //物理内存数组占用的页数
void memory_map_init()
{
    memory_map = (u8 *)memory_base;

    // 计算物理内存数组占用的页数
    memory_map_pages = div_round_up(total_pages, PAGE_SIZE);
    LOGK("memory map page count %d\n",memory_map_pages);

    free_pages -= memory_map_pages;

    //清空物理内存数组
    memset((void *)memory_map, 0, memory_map_pages * PAGE_SIZE);

    // 前1M 的内存位置 以及物理内存数组已占用的页， 已被占用
    start_page = IDX(MEMORY_BASE) + memory_map_pages;
    for (size_t i = 0; i < start_page; i++) {
        memory_map[i] = 1;
    }
    LOGK("total pages %d free pages %d\n", total_pages, free_pages);

    // 初始化内核虚拟内存位图，需要8位对齐，每个bit管理一页内存
    u32 length = (IDX(KERNEL_MEMORY_SIZE) - IDX(MEMORY_BASE)) / 8;
    bitmap_init(&kernel_map, (u8 *)KERNEL_MAP_BITS, length, IDX(MEMORY_BASE));
    bitmap_scan(&kernel_map, memory_map_pages);

}
//分配1页物理内存
static u32 get_page()
{
    for (size_t i = 0; i < total_pages; i++) {
        if (!memory_map[i]) {
            memory_map[i] = 1;
            free_pages--;
            assert(free_pages >= 0);
            u32 page = PAGE(i);
            LOGK("get page 0x%p\n", page);
            return page;
        }
        
    }
    panic("out of memory!!");
    
}
// 释放1页物理内存
static void put_page(u32 addr)
{
    ASSERT_PAGE(addr);

    u32 idx = IDX(addr);

    //idx 大于1M并且小于总页面数
    assert(idx >= start_page && idx < total_pages);

    //保证只有一个引用
    assert(memory_map[idx] >= 1);

    //物理引用-1
    memory_map[idx]--;

    //若为0， 则空闲页加1
    if (!memory_map[idx]) {
        free_pages++;
    }
    assert(free_pages > 0 && free_pages < total_pages);
    LOGK("put page 0x%p\n", addr);

}

u32 get_cr3()
{
    asm volatile("movl %cr3, %eax\n");
}
// 设置cr3寄存器， 参数是页目录的地址
void set_cr3(u32 pde)
{
    ASSERT_PAGE(pde);
    asm volatile("movl %%eax, %%cr3\n" ::"a"(pde));
}

//将 cr0的pg位置1，启用分页
static _inline void enable_page()
{
    //0x80000000
    asm volatile(
        "movl %cr0, %eax\n"
        "orl $0x80000000, %eax\n"
        "movl %eax, %cr0\n"
    );
}

static void entry_init(page_entry_t *entry, u32 index)
{
    *(u32 *)entry = 0;
    entry->present = 1;
    entry->write = 1;
    entry->user = 1;
    entry->index = index;
}


//初始化内存映射
void mapping_init()
{
    page_entry_t *pde = (page_entry_t *)KERNEL_PAGE_DIR;
    memset(pde, 0, PAGE_SIZE);

    idx_t index = 0;

    for (idx_t didx = 0; didx < (sizeof(KERNEL_PAGE_TABLE) / 4); didx++) {
        page_entry_t *pte = (page_entry_t *)KERNEL_PAGE_TABLE[didx];
        memset(pte, 0, PAGE_SIZE);
        // 映射内核页目录
        page_entry_t *dentry = &pde[didx];
        entry_init(dentry, IDX((u32)pte));
        // 映射内核页表
        for (size_t tidx = 0; tidx < 1024; tidx++, index++) {
            // 第0 页不映射，为造成缺页异常，便于排错
            if (index == 0) {
                continue;
            }
            page_entry_t *tentry = &pte[tidx];
            entry_init(tentry, index);
            memory_map[index] = 1;//设置物理内存数组，该页被占用
            
        }
    }
    // 将最后一个页表指向页目录自己，方便修改
    page_entry_t *entry = &pde[1023];
    entry_init(entry, IDX(KERNEL_PAGE_DIR));


    set_cr3((u32)pde);

    enable_page();

}
//获取页目录
static page_entry_t *get_pde()
{
    return (page_entry_t *)(0xfffff000);//对应的物理地址是0x1000
}
//获取页表
static page_entry_t *get_pte(u32 vaddr)
{
    return (page_entry_t *)(0xffc00000 | (DIDX(vaddr) << 12));
}
// 刷新块表
static void flush_tlb(u32 vaddr)
{
    asm volatile("invlpg (%0)" :: "r"(vaddr)
                : "memory");
}

// 从位图中找到cout 个连续的页
static u32 scan_page(bitmap_t *map, u32 count)
{
    assert(count > 0);
    int32 index = bitmap_scan(map, count);

    if (index == EOF) {
        panic("scan page fail ");
    }
    u32 addr = PAGE(index);
    // LOGK("scan page 0x%p count %d\n", addr, count);
    return addr;
    
}

// 与scan_page 相对，重置相应的页
static void reset_page(bitmap_t *map, u32 addr, u32 count)
{
    ASSERT_PAGE(addr);
    assert(count > 0);
    u32 index = IDX(addr);

    for (size_t i = 0; i < count; i++) {
        assert(bitmap_test(map, index + i));
        bitmap_set(map, index + i, 0);
    }
    
}

// 分配count 个连续的内核页
u32 alloc_kpage(u32 count)
{
    assert(count > 0);
    u32 vaddr = scan_page(&kernel_map, count);
    LOGK("alloc kernel page 0x%p count %d\n", vaddr, count);
    return vaddr;
}

// 释放 count 个连续的内核页
void free_kpage(u32 vaddr, u32 count)
{
    ASSERT_PAGE(vaddr);
    assert(count > 0);
    reset_page(&kernel_map, vaddr, count);
    LOGK("free kernel page 0x%p count %d\n", vaddr, count);
}

void memory_test()
{
    u32 *pages = (u32 *)(0x200000);
    u32 count = 0x6fe;
    for (size_t i = 0; i < count/2; i++)
    {
        pages[i] = alloc_kpage(2);
        LOGK("0x%x\n", i);
    }
    for (size_t i = 0; i < count/2; i++)
    {
        free_kpage(pages[i], 1);
        pages[i] = 0;
    }
    free_kpage(pages[count/2], 1);
    
}










#include <onix/global.h>
#include <onix/debug.h>
#include <onix/string.h>

descriptor_t gdt[GDT_SIZE]; //内核全局描述符表
pointer_t gdt_ptr;          //内核全局描述符指针



// 初始内核全局描述符表
void gdt_init()
{
    DEBUGK("gdt init\n");

    asm volatile("sgdt gdt_ptr");

    memcpy((void *)&gdt, (const void *)gdt_ptr.base, gdt_ptr.limit + 1);

    gdt_ptr.base = (u32)&gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;

    asm volatile("lgdt gdt_ptr\n");

}









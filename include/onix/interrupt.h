#ifndef __ONIX_INTERRUPT_H__
#define __ONIX_INTERRUPT_H__

#include <onix/types.h>

#define IDT_SIZE 256
#define IRQ_MASTER_NR 0x20 //主片起始向量号
#define IRQ_CLOCK 0 //时钟

typedef struct gate_t {
    u16 offset0;//段内偏移0-15位
    u16 selector; //代码段选择子
    u8 reserved; //保留
    u8 type : 4; //任务门、中断门，陷阱门
    u8 segment : 1;  //segment = 0 表示系统段
    u8 DPL : 2;//使用 int 指令访问的最低权限
    u8 present : 1;//是否有效
    u16 offset1;//段内偏移16-31位
} _packed gate_t;

void interrupt_init();
void send_eoi(u32 vector);
void set_interrupt_mask(u32 irq, bool enable);
void set_interrupt_handler(u32 irq, handler_t handler);

#endif
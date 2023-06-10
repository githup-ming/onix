[bits 32]

magic equ 0xe85250d6
i386 equ 0
length equ header_end - header_start

section .multiboot2
header_start:
    dd magic; 魔数
    dd i386; 32位保护模式
    dd length; 头部长度
    dd -(magic + i386 + length); 校验和

    ; 结束标记
    dw 0; type
    dw 0; flags
    dd 8; size
header_end:



extern kernel_init
extern console_init
extern memory_init
extern gdt_init

section .text
global _start
_start:
    ; mov byte [0xb8000], 'K'
    push ebx; ards_count
    push eax; magic

    call console_init; 初始化控制台
    call gdt_init; 初始化全局描述符
    call memory_init; 初始化内存
    call kernel_init; 初始化内核

    jmp $;阻塞

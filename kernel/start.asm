[bits 32]

extern kernel_init

global _start
_start:
    ; mov byte [0xb8000], 'K'

    call kernel_init
    xchg bx, bx; bochs 魔术断点

    int 0x80; 调用中断函数
    ; mov bx, 0
    ; div bx

    jmp $;阻塞

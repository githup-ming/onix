[bits 32]

extern kernel_init
extern console_init
extern memory_init

global _start
_start:
    ; mov byte [0xb8000], 'K'
    push ebx; ards_count
    push eax; magic

    call console_init; 初始化控制台
    call memory_init; 初始化内存
    ; call kernel_init; 初始化内核


    jmp $;阻塞

[org 0x1000]

dw 0x55aa; 魔数 用于判断错误

mov si, loading
call print

xchg bx, bx; bochs 魔术断点

detect_memory:
    ; 将ebx置位0
    xor ebx, ebx
    ;es: di结构体缓冲区的位置
    mov ax, 0
    mov es, ax
    mov edi, ards_buffer

    mov edx, 0x534d4150; 固定签名

.next:
    mov eax, 0xe820
    mov ecx, 20; ards 结构大小
    int 0x15; 调用0x15系统调用

    jc error; 如果cf置位，表示出错
    add di, cx

    inc word [ards_count]

    cmp ebx, 0
    jnz .next

    mov si, detecting
    call print

    xchg bx, bx; bochs 魔术断点

    ; mov byte [0xb8000], 'P'

    jmp prepare_protected_mode


jmp $; 阻塞

prepare_protected_mode:
    xchg bx,bx
    cli; 关闭中断

    in al, 0x92
    or al, 0b10
    out 0x92, al
    ;加载gdt
    lgdt [gdt_ptr]

    ;启动保护mos
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    ;用跳转刷新缓存，启用保护模式
    jmp dword code_selector:protect_mode



print:
    mov ah, 0x0e
.next:
    mov al, [si]
    cmp al, 0
    jz .done
    int 0x10
    inc si
    jmp .next
.done:
    ret

loading:
    db "loading Onix...", 10, 13, 0;\n \r
detecting:
    db "detecting memory success...", 10, 13, 0;\n \r

error:
    mov si, .msg
    call print
    hlt; 让CPU停止
    jmp $
    .msg db "loading error", 10, 13, 0;\n \r


[bits 32]
protect_mode:
    xchg bx,bx
    mov ax, data_selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss , ax; 初始化段寄存器

    mov esp, 0x10000; 修改栈顶

    mov byte [0xb8000], 'P'

    mov byte [0x200000], 'P'

jmp $; 阻塞


code_selector equ (1 << 3)
data_selector equ (2 << 3)


memory_base equ 0; 内存开始的位置；基地址

memory_limit equ ((1024 * 1024 *1024 * 4) / (1024 * 4)) -1

gdt_ptr:
    dw (gdt_end -gdt_base) -1
    dd gdt_base

gdt_base:
    dd 0, 0;NULL 描述符

gdt_code:
    dw memory_limit & 0xffff; 段界限0-15
    dw memory_base & 0xffff; 基地址0-16
    db (memory_base >> 16) & 0xff;
    db 0b_1_00_1_1_0_1_0; 存在 dpl 0 代码
    db 0b1_1_0_0_0000| (memory_limit >> 16) & 0xf;
    db (memory_base >> 24) & 0xff

gdt_data:
    dw memory_limit & 0xffff; 段界限0-15
    dw memory_base & 0xffff; 基地址0-16
    db (memory_base >> 16) & 0xff;
    db 0b_1_00_1_0_0_1_0; 存在 dpl 0 代码
    db 0b1_1_0_0_0000| (memory_limit >> 16) & 0xf;
    db (memory_base >> 24) & 0xff

gdt_end:

ards_count:
    dw 0

ards_buffer:

    
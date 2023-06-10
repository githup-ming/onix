[org 0x1000]

dw 0x55aa; 魔数 用于判断错误

mov si, loading
call print

; xchg bx, bx; bochs 魔术断点

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

    inc dword [ards_count]

    cmp ebx, 0
    jnz .next

    mov si, detecting
    call print

    ; xchg bx, bx; bochs 魔术断点

    ; 实模式下不修改段寄存器，只能访问64k内存，0xb8000,超过了64k，可以看出并没有修改成功
    ; mov byte [0xb8000], 'P'

    jmp prepare_protected_mode


jmp $; 阻塞

prepare_protected_mode:
    ; xchg bx,bx
    cli; 关闭中断

    ;打开 A20 线
    in al, 0x92
    or al, 0b10
    out 0x92, al
    ;加载gdt
    lgdt [gdt_ptr]

    ;启动保护模式，cr0 第0位置1
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
    ; xchg bx,bx
    mov ax, data_selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax; 初始化段寄存器

    mov esp, 0x10000; 修改栈顶

    mov edi, 0x10000;把硬盘中的数据读取0x1000内存中
    mov ecx, 10;硬盘的起始扇区
    mov bl, 200;读取硬盘的扇区数量

    call read_disk; 读取内核

    ;为了兼容grub
    mov eax, 0x20230422; 内核魔数
    mov ebx, ards_count; ards 数量指针 

    mov byte [0xb8000], 'L'
    ; xchg bx, bx; bochs 魔术断点

    jmp dword code_selector:0x10040; 跳转到代码段

    ud2; 表示出错


jmp $; 阻塞


read_disk:
    ;设置读写扇区的数量
    mov dx, 0x1f2
    mov al, bl
    out dx, al

    inc dx;0x1f3
    mov al, cl; 起始扇区的前8位
    out dx, al

    inc dx;0x1f4
    shr ecx, 8
    mov al, cl; 起始扇区的中8位
    out dx, al

    inc dx;0x1f5
    shr ecx, 8
    mov al, cl; 起始扇区的高8位
    out dx, al

    inc dx; 0x1f6
    shr ecx,8
    and cl, 0b1111; 将高4位置0

    mov al, 0b1110_0000
    or al, cl
    out dx, al;  主盘LBA模式

    inc dx; 0x1f7
    mov al, 0x20
    out dx, al; 读硬盘

    xor ecx, ecx; 将ecx清空
    mov cl, bl; 得到读写扇区的数量

    .read:
    push cx; 保存cx
    call .waits; 等待数据准备完毕
    call .reads; 读取一个扇区
    pop cx
    loop .read

    ret

    .waits:
        mov dx, 0x1f7
        .check:
            in al, dx
            jmp $+2; nop
            jmp $+2
            jmp $+2
            and al, 0b1000_1000
            cmp al, 0b0000_1000
            jnz .check
        ret

    .reads:
        mov dx, 0x1f0
        mov cx, 256; 一个扇区256个字节
        .readw:
            in ax, dx
            jmp $+2; nop
            jmp $+2
            jmp $+2
            mov [edi], ax
            add edi, 2
            loop .readw
        ret


code_selector equ (1 << 3)
data_selector equ (2 << 3)


memory_base equ 0; 内存开始的位置；基地址
; 4GB / 4KB - 1
memory_limit equ ((1024 * 1024 *1024 * 4) / (1024 * 4)) -1

gdt_ptr:
    dw (gdt_end -gdt_base) -1
    dd gdt_base

gdt_base: ; 第0个段全局描述符
    dd 0, 0;NULL 描述符，dd 占4个字节,两个0 占8个字节

gdt_code: ; 第1个段全局描述符
    dw memory_limit & 0xffff; 段界限0-15
    dw memory_base & 0xffff; 基地址0-15
    db (memory_base >> 16) & 0xff; 基地址16-23
    ;存在 - dlp=0 - S - 代码 - 非依从 - 可读 - 没有被访问过
    db 0b_1_00_1_1_0_1_0
    ;4k - 32bit - 非64位 - 保留位 - 段界限16-19
    db 0b1_1_0_0_0000| ((memory_limit >> 16) & 0xf)
    db (memory_base >> 24) & 0xff

gdt_data: ; 第2个段全局描述符
    dw memory_limit & 0xffff; 段界限0-15
    dw memory_base & 0xffff; 基地址0-16
    db (memory_base >> 16) & 0xff;
    ;存在 - dlp=0 - S - 数据 - 向上 - 可写 - 没有被访问过
    db 0b_1_00_1_0_0_1_0; 存在 dpl 0 代码
    db 0b1_1_0_0_0000| ((memory_limit >> 16) & 0xf)
    db (memory_base >> 24) & 0xff

gdt_end:

ards_count:
    dd 0

ards_buffer:

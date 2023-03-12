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

    mov cx, [ards_count]
    mov si, 0

.show:
    mov eax, [si + ards_buffer]; 内存位置
    mov ebx, [ards_buffer + si + 8]; 内存大小
    mov edx, [ards_buffer + si + 16]; 内存类型，1 可用，2 不可用
    add si, 20
    xchg bx, bx
    loop .show

jmp $; 阻塞

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

ards_count:
    dw 0

ards_buffer:

    
[org 0x7c00]

; 设置屏幕模式为文本模式，清除屏幕
mov ax, 3
int 0x10

;初始化段寄存器

mov ax, 0
mov ds, ax
mov es, ax
mov ss, ax
mov sp, 0x7c00

xchg bx, bx
; call interrupt; 普通调用

mov word [0x80 * 4], interrupt
mov word [0x80 * 4 + 2], 0


int 0x80; linux的系统调用号

;阻塞
jmp $

interrupt:
    mov si, string
    call print
    xchg bx, bx
    ; ret; 普通返回
    iret



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

error:
    mov si, .msg
    call print
    hlt; 让CPU停止
    jmp $
    .msg db "booting error", 10, 13, 0;\n \r


string:
    db ".", 0;\n \r
bootend:
    db "Boot Onix success", 10, 13, 0;\n \r

readend:
    db "read success", 10, 13, 0;\n \r

writeend:
    db "write success", 10, 13, 0;\n \r


;填充0
times 510 - ($ - $$) db 0

;主引导扇区的最后两个字节必须是0x55 0xaa
db 0x55, 0xaa

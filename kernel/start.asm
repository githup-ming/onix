[bits 32]

extern kernel_init

global _start
_start:
    ; mov byte [0xb8000], 'K'
    ; mov si, bootkernel
    ; call print

    call kernel_init
    xchg bx, bx; bochs 魔术断点

    jmp $;阻塞

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

bootkernel:
    db "start kernel", 10, 13, 0;\n \r
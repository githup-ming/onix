[bits 32]

section .text; text 代码段

global inb ; 将inb导出
inb:
	push	ebp
	mov	ebp, esp ; 保存栈帧

    xor eax, eax ; 将eax清零
    mov edx, [ebp + 8] ;port
    in al, dx; 将端口号dx的8bit输入到ax

    jmp $+2 ; 一点点延迟
    jmp $+2 ; 一点点延迟
    jmp $+2 ; 一点点延迟


    leave ; 恢复栈帧

    ret



global outb ; 将inb导出
outb:
	push	ebp
	mov	ebp, esp ; 保存栈帧

    mov edx, [ebp + 8]; port
    mov eax, [ebp + 12]; value
    out dx, al; 将al 的8bit 输出到 dx

    jmp $+2 ; 一点点延迟
    jmp $+2 ; 一点点延迟
    jmp $+2 ; 一点点延迟

    leave ; 恢复栈帧

    ret


global inw ; 将inw导出
inw:
	push	ebp
	mov	ebp, esp ; 保存栈帧

    xor eax, eax ; 将eax清零
    mov edx, [ebp + 8] ;port
    in ax, dx; 将端口号dx的8bit输入到ax

    jmp $+2 ; 一点点延迟
    jmp $+2 ; 一点点延迟
    jmp $+2 ; 一点点延迟


    leave ; 恢复栈帧

    ret



global outw ; 将inb导出
outw:
	push	ebp
	mov	ebp, esp ; 保存栈帧

    mov edx, [ebp + 8]; port
    mov eax, [ebp + 12]; value
    out dx, ax; 将al 的8bit 输出到 dx

    jmp $+2 ; 一点点延迟
    jmp $+2 ; 一点点延迟
    jmp $+2 ; 一点点延迟

    leave ; 恢复栈帧

    ret
[bits 32]
;中断函数入口

extern handler_table

section .text

%macro INTERRUPT_HANDLER 2
interrupt_handler_%1:
    ; xchg bx, bx; 断点
%ifn %2
    push 0x20222202; 占位
%endif
    push %1; 中断编号压入栈中
    jmp interrupt_entry
%endmacro

interrupt_entry:
;保存上文寄存器
    push ds
    push es
    push fs
    push gs
    pusha ;8个 eax ecx edx ebx esp ebp esi edi

    mov eax, [esp + 12 * 4]; esp是最后压入栈的数据，对应上面的push 1%ssss

    push eax; 向中断函数提供参数，调用函数前最后压入栈中的是函数的第一个参数

    ; 调用中断处理函数，handler_table 中储存了中断处理函数的指针
    call [handler_table + eax * 4]

global interrupt_exit
interrupt_exit:

    add esp, 4; 对应push eax
;恢复下文寄存器
    popa
    pop gs
    pop fs
    pop es
    pop ds

    add esp, 8; 对应push %1 和 error

    iret

INTERRUPT_HANDLER 0x00, 0; 除0 异常
INTERRUPT_HANDLER 0x01, 0; debug
INTERRUPT_HANDLER 0x02, 0; 除0 异常
INTERRUPT_HANDLER 0x03, 0; breakpoint
INTERRUPT_HANDLER 0x04, 0; overflow
INTERRUPT_HANDLER 0x05, 0; bound range exceeded
INTERRUPT_HANDLER 0x06, 0; invalid opcode
INTERRUPT_HANDLER 0x07, 0; device not avilable
INTERRUPT_HANDLER 0x08, 1; double fault
INTERRUPT_HANDLER 0x09, 0; 除0 异常
INTERRUPT_HANDLER 0x0a, 1; 除0 异常
INTERRUPT_HANDLER 0x0b, 1; 除0 异常
INTERRUPT_HANDLER 0x0c, 1; 除0 异常
INTERRUPT_HANDLER 0x0d, 1; 一般性保护异常
INTERRUPT_HANDLER 0x0e, 1; 除0 异常
INTERRUPT_HANDLER 0x0f, 0; 除0 异常
INTERRUPT_HANDLER 0x10, 0; 除0 异常
INTERRUPT_HANDLER 0x11, 1; 除0 异常
INTERRUPT_HANDLER 0x12, 0; 除0 异常
INTERRUPT_HANDLER 0x13, 0; 除0 异常
INTERRUPT_HANDLER 0x14, 0; 除0 异常
INTERRUPT_HANDLER 0x15, 1; 除0 异常
INTERRUPT_HANDLER 0x16, 0; reserved
INTERRUPT_HANDLER 0x17, 0; reserved
INTERRUPT_HANDLER 0x18, 0; reserved
INTERRUPT_HANDLER 0x19, 0; reserved
INTERRUPT_HANDLER 0x1a, 0; reserved
INTERRUPT_HANDLER 0x1b, 0; reserved
INTERRUPT_HANDLER 0x1c, 0; reserved
INTERRUPT_HANDLER 0x1d, 0; reserved
INTERRUPT_HANDLER 0x1e, 0; reserved
INTERRUPT_HANDLER 0x1f, 0; reserved
; 0 - 0x1f 是异常，0x20 - 0xff 是外中断或者软中断
INTERRUPT_HANDLER 0x20, 0; clock 时钟中断
INTERRUPT_HANDLER 0x21, 0; 除0 异常
INTERRUPT_HANDLER 0x22, 0; 除0 异常
INTERRUPT_HANDLER 0x23, 0; 除0 异常
INTERRUPT_HANDLER 0x24, 0; 除0 异常
INTERRUPT_HANDLER 0x25, 0; 除0 异常
INTERRUPT_HANDLER 0x26, 0; reserved
INTERRUPT_HANDLER 0x27, 0; reserved
INTERRUPT_HANDLER 0x28, 0; RTC 中断
INTERRUPT_HANDLER 0x29, 0; reserved
INTERRUPT_HANDLER 0x2a, 0; reserved
INTERRUPT_HANDLER 0x2b, 0; reserved
INTERRUPT_HANDLER 0x2c, 0; reserved
INTERRUPT_HANDLER 0x2d, 0; reserved
INTERRUPT_HANDLER 0x2e, 0; reserved
INTERRUPT_HANDLER 0x2f, 0; reserved


; 下面的数组记录了每个中断入口函数的指针
section .data
global handler_entry_table
handler_entry_table:
    dd interrupt_handler_0x00
    dd interrupt_handler_0x01
    dd interrupt_handler_0x02
    dd interrupt_handler_0x03
    dd interrupt_handler_0x04
    dd interrupt_handler_0x05
    dd interrupt_handler_0x06
    dd interrupt_handler_0x07
    dd interrupt_handler_0x08
    dd interrupt_handler_0x09
    dd interrupt_handler_0x0a
    dd interrupt_handler_0x0b
    dd interrupt_handler_0x0c
    dd interrupt_handler_0x0d
    dd interrupt_handler_0x0e
    dd interrupt_handler_0x0f
    dd interrupt_handler_0x10
    dd interrupt_handler_0x11
    dd interrupt_handler_0x12
    dd interrupt_handler_0x13
    dd interrupt_handler_0x14
    dd interrupt_handler_0x15
    dd interrupt_handler_0x16
    dd interrupt_handler_0x17
    dd interrupt_handler_0x18
    dd interrupt_handler_0x19
    dd interrupt_handler_0x1a
    dd interrupt_handler_0x1b
    dd interrupt_handler_0x1c
    dd interrupt_handler_0x1d
    dd interrupt_handler_0x1e
    dd interrupt_handler_0x1f

    dd interrupt_handler_0x20
    dd interrupt_handler_0x21
    dd interrupt_handler_0x22
    dd interrupt_handler_0x23
    dd interrupt_handler_0x24
    dd interrupt_handler_0x25
    dd interrupt_handler_0x26
    dd interrupt_handler_0x27
    dd interrupt_handler_0x28
    dd interrupt_handler_0x29
    dd interrupt_handler_0x2a
    dd interrupt_handler_0x2b
    dd interrupt_handler_0x2c
    dd interrupt_handler_0x2d
    dd interrupt_handler_0x2e
    dd interrupt_handler_0x2f
 



 section .text

 extern syscall_check
 extern syscall_table
 global syscall_handler
 syscall_handler:
    ; xchg bx, bx

    ; 验证系统调用号
    push eax
    call syscall_check
    add esp, 4

    push 0x20222202; 错误码

    push 0x80; 中断号

    ; 保存上文寄存器信息
    push ds
    push es
    push fs
    push gs
    pusha

    push 0x80; 向中断处理函数传递中断号
    ; xchg bx, bx

    push edx; 第三个参数
    push ecx;第二个参数
    push ebx; 第一个参数

    ; 调用系统调用处理函数
    call [syscall_table + eax * 4]

    ; xchg bx, bx
    add esp, 12

    ; 修改栈中 eax ，设置系统调用返回值
    mov dword [esp + 8 * 4], eax

    ; 跳转到中断返回
    jmp interrupt_exit

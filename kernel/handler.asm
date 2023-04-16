[bits 32]
;中断函数入口

extern handler_table

section .text

%macro INTERRUPT_HANDLER 2
interrupt_handler_%1:
%ifn %2
    push 0x20222202; 占位
%endif
    push %1; 中断编号压入栈中
    jmp interrupt_entry
%endmacro

interrupt_entry:

    mov eax, [esp]; esp是最后压入栈的数据，对应上面的push 1%

    ; 调用中断处理函数，handler_table 中储存了中断处理函数的指针
    call [handler_table + eax * 4]
    ; 对应push %1 调用结束恢复栈
    add esp, 8
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
INTERRUPT_HANDLER 0x0d, 1; 除0 异常
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

INTERRUPT_HANDLER 0x20, 0; 除0 异常
INTERRUPT_HANDLER 0x21, 1; 除0 异常
INTERRUPT_HANDLER 0x22, 0; 除0 异常
INTERRUPT_HANDLER 0x23, 0; 除0 异常
INTERRUPT_HANDLER 0x24, 0; 除0 异常
INTERRUPT_HANDLER 0x25, 1; 除0 异常
INTERRUPT_HANDLER 0x26, 0; reserved
INTERRUPT_HANDLER 0x27, 0; reserved
INTERRUPT_HANDLER 0x28, 0; reserved
INTERRUPT_HANDLER 0x29, 0; reserved
INTERRUPT_HANDLER 0x2a, 0; reserved
INTERRUPT_HANDLER 0x2b, 0; reserved
INTERRUPT_HANDLER 0x2c, 0; reserved
INTERRUPT_HANDLER 0x2d, 0; reserved
INTERRUPT_HANDLER 0x2e, 0; reserved
INTERRUPT_HANDLER 0x2f, 0; reserved

INTERRUPT_HANDLER 0x30, 0; 除0 异常
INTERRUPT_HANDLER 0x31, 1; 除0 异常
INTERRUPT_HANDLER 0x32, 0; 除0 异常
INTERRUPT_HANDLER 0x33, 0; 除0 异常
INTERRUPT_HANDLER 0x34, 0; 除0 异常
INTERRUPT_HANDLER 0x35, 1; 除0 异常
INTERRUPT_HANDLER 0x36, 0; reserved
INTERRUPT_HANDLER 0x37, 0; reserved
INTERRUPT_HANDLER 0x38, 0; reserved
INTERRUPT_HANDLER 0x39, 0; reserved
INTERRUPT_HANDLER 0x3a, 0; reserved
INTERRUPT_HANDLER 0x3b, 0; reserved
INTERRUPT_HANDLER 0x3c, 0; reserved
INTERRUPT_HANDLER 0x3d, 0; reserved
INTERRUPT_HANDLER 0x3e, 0; reserved
INTERRUPT_HANDLER 0x3f, 0; reserved

INTERRUPT_HANDLER 0x40, 0; 除0 异常
INTERRUPT_HANDLER 0x41, 1; 除0 异常
INTERRUPT_HANDLER 0x42, 0; 除0 异常
INTERRUPT_HANDLER 0x43, 0; 除0 异常
INTERRUPT_HANDLER 0x44, 0; 除0 异常
INTERRUPT_HANDLER 0x45, 1; 除0 异常
INTERRUPT_HANDLER 0x46, 0; reserved
INTERRUPT_HANDLER 0x47, 0; reserved
INTERRUPT_HANDLER 0x48, 0; reserved
INTERRUPT_HANDLER 0x49, 0; reserved
INTERRUPT_HANDLER 0x4a, 0; reserved
INTERRUPT_HANDLER 0x4b, 0; reserved
INTERRUPT_HANDLER 0x4c, 0; reserved
INTERRUPT_HANDLER 0x4d, 0; reserved
INTERRUPT_HANDLER 0x4e, 0; reserved
INTERRUPT_HANDLER 0x4f, 0; reserved

INTERRUPT_HANDLER 0x50, 0; 除0 异常
INTERRUPT_HANDLER 0x51, 1; 除0 异常
INTERRUPT_HANDLER 0x52, 0; 除0 异常
INTERRUPT_HANDLER 0x53, 0; 除0 异常
INTERRUPT_HANDLER 0x54, 0; 除0 异常
INTERRUPT_HANDLER 0x55, 1; 除0 异常
INTERRUPT_HANDLER 0x56, 0; reserved
INTERRUPT_HANDLER 0x57, 0; reserved
INTERRUPT_HANDLER 0x58, 0; reserved
INTERRUPT_HANDLER 0x59, 0; reserved
INTERRUPT_HANDLER 0x5a, 0; reserved
INTERRUPT_HANDLER 0x5b, 0; reserved
INTERRUPT_HANDLER 0x5c, 0; reserved
INTERRUPT_HANDLER 0x5d, 0; reserved
INTERRUPT_HANDLER 0x5e, 0; reserved
INTERRUPT_HANDLER 0x5f, 0; reserved

INTERRUPT_HANDLER 0x60, 0; 除0 异常
INTERRUPT_HANDLER 0x61, 1; 除0 异常
INTERRUPT_HANDLER 0x62, 0; 除0 异常
INTERRUPT_HANDLER 0x63, 0; 除0 异常
INTERRUPT_HANDLER 0x64, 0; 除0 异常
INTERRUPT_HANDLER 0x65, 1; 除0 异常
INTERRUPT_HANDLER 0x66, 0; reserved
INTERRUPT_HANDLER 0x67, 0; reserved
INTERRUPT_HANDLER 0x68, 0; reserved
INTERRUPT_HANDLER 0x69, 0; reserved
INTERRUPT_HANDLER 0x6a, 0; reserved
INTERRUPT_HANDLER 0x6b, 0; reserved
INTERRUPT_HANDLER 0x6c, 0; reserved
INTERRUPT_HANDLER 0x6d, 0; reserved
INTERRUPT_HANDLER 0x6e, 0; reserved
INTERRUPT_HANDLER 0x6f, 0; reserved

INTERRUPT_HANDLER 0x70, 0; 除0 异常
INTERRUPT_HANDLER 0x71, 1; 除0 异常
INTERRUPT_HANDLER 0x72, 0; 除0 异常
INTERRUPT_HANDLER 0x73, 0; 除0 异常
INTERRUPT_HANDLER 0x74, 0; 除0 异常
INTERRUPT_HANDLER 0x75, 1; 除0 异常
INTERRUPT_HANDLER 0x76, 0; reserved
INTERRUPT_HANDLER 0x77, 0; reserved
INTERRUPT_HANDLER 0x78, 0; reserved
INTERRUPT_HANDLER 0x79, 0; reserved
INTERRUPT_HANDLER 0x7a, 0; reserved
INTERRUPT_HANDLER 0x7b, 0; reserved
INTERRUPT_HANDLER 0x7c, 0; reserved
INTERRUPT_HANDLER 0x7d, 0; reserved
INTERRUPT_HANDLER 0x7e, 0; reserved
INTERRUPT_HANDLER 0x7f, 0; reserved

INTERRUPT_HANDLER 0x80, 0; 除0 异常
INTERRUPT_HANDLER 0x81, 1; 除0 异常
INTERRUPT_HANDLER 0x82, 0; 除0 异常
INTERRUPT_HANDLER 0x83, 0; 除0 异常
INTERRUPT_HANDLER 0x84, 0; 除0 异常
INTERRUPT_HANDLER 0x85, 1; 除0 异常
INTERRUPT_HANDLER 0x86, 0; reserved
INTERRUPT_HANDLER 0x87, 0; reserved
INTERRUPT_HANDLER 0x88, 0; reserved
INTERRUPT_HANDLER 0x89, 0; reserved
INTERRUPT_HANDLER 0x8a, 0; reserved
INTERRUPT_HANDLER 0x8b, 0; reserved
INTERRUPT_HANDLER 0x8c, 0; reserved
INTERRUPT_HANDLER 0x8d, 0; reserved
INTERRUPT_HANDLER 0x8e, 0; reserved
INTERRUPT_HANDLER 0x8f, 0; reserved

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
 
    dd interrupt_handler_0x30
    dd interrupt_handler_0x31
    dd interrupt_handler_0x32
    dd interrupt_handler_0x33
    dd interrupt_handler_0x34
    dd interrupt_handler_0x35
    dd interrupt_handler_0x36
    dd interrupt_handler_0x37
    dd interrupt_handler_0x38
    dd interrupt_handler_0x39
    dd interrupt_handler_0x3a
    dd interrupt_handler_0x3b
    dd interrupt_handler_0x3c
    dd interrupt_handler_0x3d
    dd interrupt_handler_0x3e
    dd interrupt_handler_0x3f

    dd interrupt_handler_0x40
    dd interrupt_handler_0x41
    dd interrupt_handler_0x42
    dd interrupt_handler_0x43
    dd interrupt_handler_0x44
    dd interrupt_handler_0x45
    dd interrupt_handler_0x46
    dd interrupt_handler_0x47
    dd interrupt_handler_0x48
    dd interrupt_handler_0x49
    dd interrupt_handler_0x4a
    dd interrupt_handler_0x4b
    dd interrupt_handler_0x4c
    dd interrupt_handler_0x4d
    dd interrupt_handler_0x4e
    dd interrupt_handler_0x4f

    dd interrupt_handler_0x50
    dd interrupt_handler_0x51
    dd interrupt_handler_0x52
    dd interrupt_handler_0x53
    dd interrupt_handler_0x54
    dd interrupt_handler_0x55
    dd interrupt_handler_0x56
    dd interrupt_handler_0x57
    dd interrupt_handler_0x58
    dd interrupt_handler_0x59
    dd interrupt_handler_0x5a
    dd interrupt_handler_0x5b
    dd interrupt_handler_0x5c
    dd interrupt_handler_0x5d
    dd interrupt_handler_0x5e
    dd interrupt_handler_0x5f

    dd interrupt_handler_0x60
    dd interrupt_handler_0x61
    dd interrupt_handler_0x62
    dd interrupt_handler_0x63
    dd interrupt_handler_0x64
    dd interrupt_handler_0x65
    dd interrupt_handler_0x66
    dd interrupt_handler_0x67
    dd interrupt_handler_0x68
    dd interrupt_handler_0x69
    dd interrupt_handler_0x6a
    dd interrupt_handler_0x6b
    dd interrupt_handler_0x6c
    dd interrupt_handler_0x6d
    dd interrupt_handler_0x6e
    dd interrupt_handler_0x6f

    dd interrupt_handler_0x70
    dd interrupt_handler_0x71
    dd interrupt_handler_0x72
    dd interrupt_handler_0x73
    dd interrupt_handler_0x74
    dd interrupt_handler_0x75
    dd interrupt_handler_0x76
    dd interrupt_handler_0x77
    dd interrupt_handler_0x78
    dd interrupt_handler_0x79
    dd interrupt_handler_0x7a
    dd interrupt_handler_0x7b
    dd interrupt_handler_0x7c
    dd interrupt_handler_0x7d
    dd interrupt_handler_0x7e
    dd interrupt_handler_0x7f

    dd interrupt_handler_0x80
    dd interrupt_handler_0x81
    dd interrupt_handler_0x82
    dd interrupt_handler_0x83
    dd interrupt_handler_0x84
    dd interrupt_handler_0x85
    dd interrupt_handler_0x86
    dd interrupt_handler_0x87
    dd interrupt_handler_0x88
    dd interrupt_handler_0x89
    dd interrupt_handler_0x8a
    dd interrupt_handler_0x8b
    dd interrupt_handler_0x8c
    dd interrupt_handler_0x8d
    dd interrupt_handler_0x8e
    dd interrupt_handler_0x8f



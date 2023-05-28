[bits 32]

section .text

global task_switch
task_switch:
    push ebp
    mov ebp, esp

    push ebx
    push esi
    push edi; 把ebp ebx esi edi 保存在栈上，然后才能切换到下文的栈

    mov eax, esp
    and eax, 0xfffff000; cuttent
    mov [eax], esp; 把栈的栈顶地址保存在1页开始的位置，1页大小是0x1000字节

    mov eax, [ebp + 8]; next 传进来的next为1页开始的地址，1页开始的位置保存的是栈顶
    mov esp, [eax]; 切换为next的栈

    pop edi; 把next的edi恢复
    pop esi; 把next的esi恢复
    pop ebx; 把next的ebx恢复
    pop ebp; 把next的ebp恢复

    ret
[bits 32]

section .text

global main
main:

    ; write(stdout, message, len);
    mov eax, 4; write
    mov ebx, 1; stdout
    mov ecx, message; buf
    mov edx, message.end - message
    int 0x80

    ; exit(0)
    mov eax, 1; exit
    mov ebx, 0;
    int 0x80 


section .data
message:
    db "hello world", 13, 10, 0
.end:
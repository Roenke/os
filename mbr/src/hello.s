.code16gcc
.text
     .globl _start;

_start:
        movw    $0x7c00, %sp
        movw    $msg, %si 
print:
        movw    length, %ecx
        xor     %ax, %ax
        movw    %ax, %ds
        lodsb
        cmpb    $0x0, %al
        je      loop
        movw    $0x7, %bx
        movb    $0xe, %ah
        int     $0x10
        jmp     print

loop:
        jmp loop
msg:
        .asciz  "Hello, world!"
length:
        .long . - msg

        . = _start + 510
        .byte 0x55
        .byte 0xaa

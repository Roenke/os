.code16gcc
.text
     .globl _start;

_start:
        movw    $0x7c00, %sp
        movw    $msg, %si 
print:
        lodsb                   # Load next byte from %si buffer into %al
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

        . = _start + 510     #mov to 510th byte from 0 pos
        .byte 0x55           #append boot signature
        .byte 0xaa

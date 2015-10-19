.text
     .globl start;
        .long   0x1BADB002
        .long   0x0
        .long   -(0x1BADB002)
start:
        movl    $msg, %esi
        movl    $0xB8000, %ebx
print:
        movw    length, %ecx
        xor     %ax, %ax
        movw    %ax, %ds

        lodsb
        cmpb    $0x0, %al
        je      loop
        movb    %al, (%ebx)
        inc     %ebx
        movb    $0xF0, (%ebx)
        inc     %ebx
        jmp     print
loop:
        jmp loop
msg:
        .asciz  "Hello, world!"
length:
        .long . - msg
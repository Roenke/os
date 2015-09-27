.text
     .globl start;
        .long   0x1BADB002
        .long   0x3
        .long   -(0x1BADB005)
start:
        movl    $msg, %esi
        movl    $0xB8000, %ebx
print:
        lodsb
        cmpb    $0x0, %al
        je      loop
        movb    %al, (%ebx)
        inc     %bx
        movb    $0xF0, (%ebx)
        inc     %ebx
        jmp     print
loop:
        jmp loop
msg:
        .asciz  "Hello, world!"
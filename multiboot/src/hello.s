.text
     .globl start;

start:
        movl    $msg, %esi
        movl    $0xB8000, %ebx
print:
        lodsb                   # Load next byte from %si buffer into %al
        cmpb    $0x0, %al
        je      loop
        movb    $0xB0, (%bx)
        inc     %bx
        movb    %al, (%bx)
        inc     %bx
        jmp     print
loop:
        jmp loop
msg:
        .asciz  "Hello, world!"

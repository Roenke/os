.text
     .globl start;
        .long   0x1BADB002
        .long   0x2
        .long   -(0x1BADB002 + 0x2)
start:
        movl $(stack + 0x4000), %esp

        pushl %ebx
        call cmain

.comm   stack, 0x4000

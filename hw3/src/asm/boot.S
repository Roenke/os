.text
     .globl start;
        .long   0x1BADB002
        .long   0x0
        .long   -(0x1BADB002)
start:
        movl $(stack + 0x4000), %esp

        call cmain

.comm   stack, 0x4000

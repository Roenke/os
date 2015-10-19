.globl load_idt_asm
load_idt_asm:
   movl 4(%esp), %eax 
   lidt (%eax)
   ret

.globl load_gdt_asm
load_gdt_asm:
   movl 4(%esp),%eax 
   lgdt (%eax)
   movw $0x10, %ax 
   movw %ax, %ds
   movw %ax, %es
   movw %ax, %fs
   movw %ax, %gs
   movw %ax, %ss
   ljmp $0x08,$.marker 
.marker:
   ret

.globl load_idt_asm
load_idt_asm:
   movl 4(%esp), %eax    # Указатель на структуру IDT, переданный по стеку
   lidt (%eax)          
   ret


.globl load_gdt_asm
load_gdt_asm:
   movl 4(%esp),%eax     # Аналогичный указатель на структуру GDT
   lgdt (%eax)              

   movw $0x10, %ax       # 0x10 - смешение на Data Segment
   movw %ax, %ds          
   movw %ax, %es
   movw %ax, %fs
   movw %ax, %gs
   movw %ax, %ss
   ljmp $0x08,$.marker   # 0x08 - смещение на Code Segment. В CS писать напрямую нельзя - нужно прыгать!
.marker:
   ret

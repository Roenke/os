
secondary_handler:
    # Стек Data_ERR, INT_NO, AX, CX, DX, BX, original SP, BP, SI, DI, DS

    # Сохраним edi,esi,ebp,esp,ebx,edx,ecx,eax
    pusha                    

    # Сохраняем состояние ds
    movw %ds, %ax
    push %eax
    movw $0x10, %ax        
    movw %ax, %ds          
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    call isr_handler
    pop %eax          

    movw %ax, %ds          
    movw %ax, %es 
    movw %ax, %fs   
    movw %ax, %gs
    movw %ax, %ss

    popa

    add $8, %esp        

    sti
    iret           

# Первичные обработчкики прерываний
.align 16
.globl isr0
isr0:
    cli
    pushl $0
    pushl $0
    jmp secondary_handler

.globl isr1
isr1:
    cli
    pushl $1
    pushl $1
    jmp secondary_handler

.globl isr2
isr2:
    cli
    pushl $2
    pushl $2
    jmp secondary_handler

.globl isr3
isr3:
    cli
    pushl $3
    pushl $3
    jmp secondary_handler

.globl isr4
isr4:
    cli
    pushl $4
    pushl $4
    jmp secondary_handler

.globl isr5
isr5:
    cli
    pushl $5
    pushl $5
    jmp secondary_handler

.globl isr6
isr6:
    cli
    pushl $6
    pushl $6
    jmp secondary_handler

.globl isr7
isr7:
    cli
    pushl $7
    pushl $7
    jmp secondary_handler

.globl isr8
isr8:
    cli
    pushl $8
    jmp secondary_handler

.globl isr9
isr9:
    cli
    pushl $9
    pushl $9
    jmp secondary_handler

.globl isr10
isr10:
    cli
    pushl $10
    jmp secondary_handler

.globl isr11
isr11:
    cli
    pushl $11
    jmp secondary_handler

.globl isr12
isr12:
    cli
    pushl $12
    jmp secondary_handler

.globl isr13
isr13:
    cli
    pushl $13
    jmp secondary_handler

.globl isr14
isr14:
    cli
    pushl $14
    jmp secondary_handler

.globl isr15
isr15:
    cli
    pushl $15
    pushl $15
    jmp secondary_handler

.globl isr16
isr16:
    cli
    pushl $16
    pushl $16
    jmp secondary_handler

.globl isr17
isr17:
    cli
    pushl $17
    pushl $17
    jmp secondary_handler

.globl isr18
isr18:
    cli
    pushl $18
    pushl $18
    jmp secondary_handler

.globl isr19
isr19:
    cli
    pushl $19
    pushl $19
    jmp secondary_handler

.globl isr20
isr20:
    cli
    pushl $20
    pushl $20
    jmp secondary_handler

.globl isr21
isr21:
    cli
    pushl $21
    pushl $21
    jmp secondary_handler

.globl isr22
isr22:
    cli
    pushl $22
    pushl $22
    jmp secondary_handler

.globl isr23
isr23:
    cli
    pushl $23
    pushl $23
    jmp secondary_handler

.globl isr24
isr24:
    cli
    pushl $24
    pushl $24
    jmp secondary_handler

.globl isr25
isr25:
    cli
    pushl $25
    pushl $25
    jmp secondary_handler

.globl isr26
isr26:
    cli
    pushl $26
    pushl $26
    jmp secondary_handler

.globl isr27
isr27:
    cli
    pushl $27
    pushl $27
    jmp secondary_handler

.globl isr28
isr28:
    cli
    pushl $28
    pushl $28
    jmp secondary_handler

.globl isr29
isr29:
    cli
    pushl $29
    pushl $29
    jmp secondary_handler

.globl isr30
isr30:
    cli
    pushl $30
    pushl $30
    jmp secondary_handler

.globl isr31
isr31:
    cli
    pushl $31
    pushl $31
    jmp secondary_handler

.globl isr32
isr32:
    cli
    pushl $32
    pushl $32
    jmp secondary_handler
  
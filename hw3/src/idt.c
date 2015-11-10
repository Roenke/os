#include "includes/idt.h"
#include "includes/int_types.h"
#include "includes/helpers.h"

extern void isr0  ();
extern void isr1  ();
extern void isr2  ();
extern void isr3  ();
extern void isr4  ();
extern void isr5  ();
extern void isr6  ();
extern void isr7  ();
extern void isr8  ();
extern void isr9  ();
extern void isr10 ();
extern void isr11 ();
extern void isr12 ();
extern void isr13 ();
extern void isr14 ();
extern void isr15 ();
extern void isr16 ();
extern void isr17 ();
extern void isr18 ();
extern void isr19 ();
extern void isr20 ();
extern void isr21 ();
extern void isr22 ();
extern void isr23 ();
extern void isr24 ();
extern void isr25 ();
extern void isr26 ();
extern void isr27 ();
extern void isr28 ();
extern void isr29 ();
extern void isr30 ();
extern void isr31 ();
extern void isr32 ();

extern void load_idt_asm();

struct idt_record_struct
{
   uint16_t base_low;
   uint16_t selector;
   uint8_t  zero_byte;
   uint8_t  flags;
   uint16_t base_high; 
} __attribute__((packed));
typedef struct idt_record_struct idt_record_t;

struct idt_ptr_struct
{
   uint16_t limit;
   uint32_t base;                
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

idt_record_t    idt[256];
idt_ptr_t       idt_ptr;

void idt_insert_record(uint8_t index, uint32_t base, uint16_t selector, uint8_t flags)
{
    idt[index].base_low  = base & 0xFFFF;
    idt[index].selector  = selector;
    idt[index].zero_byte = 0;
    idt[index].flags     = flags;
    idt[index].base_high = (base >> 16) & 0xFFFF;
}

void init_idt()
{
    idt_ptr.limit = sizeof(idt_record_t) * 256 -1;
    idt_ptr.base  = (uint32_t)&idt;

    memset((uint8_t*)&idt, 0, sizeof(idt_record_t) * 256);

    idt_insert_record( 0,  (uint32_t)isr0  , 0x08, 0x8E);
    idt_insert_record( 1,  (uint32_t)isr1  , 0x08, 0x8E);
    idt_insert_record( 2,  (uint32_t)isr2  , 0x08, 0x8E);
    idt_insert_record( 3,  (uint32_t)isr3  , 0x08, 0x8E);
    idt_insert_record( 4,  (uint32_t)isr4  , 0x08, 0x8E);
    idt_insert_record( 5,  (uint32_t)isr5  , 0x08, 0x8E);
    idt_insert_record( 6,  (uint32_t)isr6  , 0x08, 0x8E);
    idt_insert_record( 7,  (uint32_t)isr7  , 0x08, 0x8E);
    idt_insert_record( 8,  (uint32_t)isr8  , 0x08, 0x8E);
    idt_insert_record( 9,  (uint32_t)isr9  , 0x08, 0x8E);
    idt_insert_record( 10, (uint32_t)isr10 , 0x08, 0x8E);
    idt_insert_record( 11, (uint32_t)isr11 , 0x08, 0x8E);
    idt_insert_record( 12, (uint32_t)isr12 , 0x08, 0x8E);
    idt_insert_record( 13, (uint32_t)isr13 , 0x08, 0x8E);
    idt_insert_record( 14, (uint32_t)isr14 , 0x08, 0x8E);
    idt_insert_record( 15, (uint32_t)isr15 , 0x08, 0x8E);
    idt_insert_record( 16, (uint32_t)isr16 , 0x08, 0x8E);
    idt_insert_record( 17, (uint32_t)isr17 , 0x08, 0x8E);
    idt_insert_record( 18, (uint32_t)isr18 , 0x08, 0x8E);
    idt_insert_record( 19, (uint32_t)isr19 , 0x08, 0x8E);
    idt_insert_record( 20, (uint32_t)isr20 , 0x08, 0x8E);
    idt_insert_record( 21, (uint32_t)isr21 , 0x08, 0x8E);
    idt_insert_record( 22, (uint32_t)isr22 , 0x08, 0x8E);
    idt_insert_record( 23, (uint32_t)isr23 , 0x08, 0x8E);
    idt_insert_record( 24, (uint32_t)isr24 , 0x08, 0x8E);
    idt_insert_record( 25, (uint32_t)isr25 , 0x08, 0x8E);
    idt_insert_record( 26, (uint32_t)isr26 , 0x08, 0x8E);
    idt_insert_record( 27, (uint32_t)isr27 , 0x08, 0x8E);
    idt_insert_record( 28, (uint32_t)isr28 , 0x08, 0x8E);
    idt_insert_record( 29, (uint32_t)isr29 , 0x08, 0x8E);
    idt_insert_record( 30, (uint32_t)isr30 , 0x08, 0x8E);
    idt_insert_record( 31, (uint32_t)isr31 , 0x08, 0x8E);
    idt_insert_record( 32, (uint32_t)isr32 , 0x08, 0x8E);

    load_idt_asm((uint32_t)&idt_ptr);
}

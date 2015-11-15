#include "includes/gdt.h"
#include "includes/screen.h"
#include "includes/int_types.h"

extern void load_gdt_asm(uint32_t);

struct gdt_record_struct
{
   uint16_t limit_low;
   uint16_t base_low;
   uint8_t  base_middle;
   uint8_t  flags;           
   uint8_t  gran; 
   uint8_t  base_high; 
} __attribute__((packed));
typedef struct gdt_record_struct gdt_record_t;


struct gdt_ptr_struct
{
   uint16_t limit;
   uint32_t base;
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

gdt_record_t    gdt[3];
gdt_ptr_t       gdt_ptr;

void gdt_insert_record(int32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[index].limit_low  = limit & 0xFFFF;
    gdt[index].flags      = access;

    gdt[index].gran  = (limit >> 16) & 0x0F;
    gdt[index].gran |= gran          & 0xF0;

    gdt[index].base_low    =  base         & 0xFFFF;
    gdt[index].base_middle = (base >> 16)  & 0xFF;
    gdt[index].base_high   = (base >> 24)  & 0xFF;
}

void init_gdt()
{
    gdt_ptr.limit = (sizeof(gdt_record_t) * 3) - 1;
    gdt_ptr.base  = (uint32_t)gdt;

    gdt_insert_record(0, 0, 0         , 0   , 0   ); // Null segment
    gdt_insert_record(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    gdt_insert_record(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

    load_gdt_asm((uint32_t)&gdt_ptr);
}

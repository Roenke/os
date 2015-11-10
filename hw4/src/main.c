#include "includes/screen.h"
#include "includes/desc_tables.h"
#include "includes/pic.h"
#include "includes/int_types.h"

struct multiboot_info{
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
}__attribute__((packed));
typedef struct multiboot_info multiboot_info_t;

void cmain(multiboot_info_t* mis)
{
    cls();
    
    init_tables();
    init_timer();

    printf("flags = %x\n", mis->flags);
    if(mis->flags & 0x4){
        printf("cmdline is valid\n");
        printf("Value of cmd: ");
        printf((const char*) mis->cmdline);
        printf("\n");
    }
    else{
        printf("cmdline does mot valid\n");
    }

    if(mis->flags & (1 << 6)){
        printf("mmap is available\n");
        printf("Length = %x\n", mis->mmap_length);
        printf("Address = %x\n", mis->mmap_addr);
    }
    else{
        printf("mmap not available\n");
    }

    //asm volatile ("sti");
    
    while(1);
}

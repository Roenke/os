#include "includes/info.h"

#define CMDLINE_IS_VALID 0x04
#define MMAP_IS_VALID    0x40

void about_cmdline(multiboot_info_t *mis)
{
    if(mis->flags & CMDLINE_IS_VALID)
    {
        printf("cmdline: %s\n", (const char*) mis->cmdline);
    }
    else
    {
        printf("cmdline does mot valid\n");
    }
}

void about_mmap(multiboot_info_t *mis)
{
    if(mis->flags & MMAP_IS_VALID)
    {
        uint32_t addr = mis->mmap_addr;
        const uint32_t last_addr = addr + mis->mmap_length;

        mmap_info_t* mmap_info_prt = addr;
        while(mmap_info_prt < last_addr)
        {
            printf("length = %x\n", mmap_info_prt->lenght);
            printf("size = %d\n", mmap_info_prt->size);
            printf("type = %d\n", mmap_info_prt->type);
            mmap_info_prt = (uint32_t)mmap_info_prt + (mmap_info_prt->size + 4);
            printf("new_address = %x\n", mmap_info_prt);
        }
    }
    else
    {
        printf("mmap not available\n");
    }
}

void about_apic()
{
    
}

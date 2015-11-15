#include "includes/info.h"

#define CMDLINE_IS_VALID  0x04
#define MMAP_IS_VALID     0x40

void about_cmdline(multiboot_info_t *mis)
{
    if(mis->flags & CMDLINE_IS_VALID)
    {
        printf("cmdline: %s\n", (const char*) mis->cmdline);
    }
    else
    {
        printf("cmdline not available\n");
    }
}

void about_mmap(multiboot_info_t *mis)
{
    if(mis->flags & MMAP_IS_VALID)
    {
        const uint32_t last_addr = mis->mmap_addr + mis->mmap_length;
        uint32_t* upper_bytes;
        uint32_t* down_bytes;
        uint64_t end_addr;
        mmap_info_t* mmap_info_prt = (mmap_info_t*)mis->mmap_addr;
        while((uint32_t)mmap_info_prt < last_addr)
        {
            down_bytes = (uint32_t*)&(mmap_info_prt->base_addr);
            upper_bytes = down_bytes + 1;
            printf("memory-range: 0x%x%x - ", *upper_bytes, *down_bytes);

            end_addr = mmap_info_prt->base_addr + mmap_info_prt->length - 1;
            down_bytes = (uint32_t*)&(end_addr);
            upper_bytes = down_bytes + 1;
            printf("0x%x%x, type %d\n", *upper_bytes, *down_bytes, mmap_info_prt->type);

            mmap_info_prt = (mmap_info_t*)
                ((uint32_t)mmap_info_prt + (mmap_info_prt->size + 4));
        }
    }
    else
    {
        printf("mmap not available\n");
    }
}

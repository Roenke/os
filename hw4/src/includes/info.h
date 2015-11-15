#ifndef INFO_H
#define INFO_H
#include "int_types.h"

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

struct mmap_info{
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
}__attribute__((packed));
typedef struct mmap_info mmap_info_t;

void about_cmdline(multiboot_info_t*);
void about_mmap(multiboot_info_t*);

#endif /* INFO_H */

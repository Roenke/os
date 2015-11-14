#include "includes/info.h"

#define CMDLINE_IS_VALID  0x04
#define MMAP_IS_VALID     0x40
#define MAGIC_POINTER     0x40E
#define EBDA_FIND_LENGTH  1024
#define MEM_REG_BEGIN     0x000E0000
#define MEM_REG_END       0x000FFFFF

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

struct rsdp_descriptor 
{
 char signature[8];
 uint8_t checksum;
 char oemid[6];
 uint8_t revision;
 uint32_t rsdt_address;
} __attribute__ ((packed));
typedef struct rsdp_descriptor rsdp_descriptor_t;

struct sdt_header
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
};
typedef struct sdt_header sdt_header_t;

struct madt_entry_header
{
    uint8_t entry_type;
    uint8_t record_length;
}__attribute__ ((packed));
typedef struct madt_entry_header madt_entry_header_t;

struct madt {
    sdt_header_t header;
    uint32_t local_controller_address;
    uint32_t flags;
    madt_entry_header_t entry;
}__attribute__ ((packed));
typedef struct madt madt_t;

struct rsdt
{
    sdt_header_t header;
    uint32_t pointer_to_other_sdt[]; 
}__attribute__ ((packed));
typedef struct rsdt rsdt_t;

struct proc_local_apic
{
    madt_entry_header_t header;
    uint8_t  acpi_processor_id;
    uint8_t  apic_id;
    uint32_t flags;
}__attribute__ ((packed));
typedef struct proc_local_apic proc_local_apic_t;

struct io_apic
{
    madt_entry_header_t header;
    uint8_t io_apics_id;
    uint8_t reserved;
    uint32_t io_apic_address;
    uint32_t global_system_interrupt_base;
}__attribute__ ((packed));
typedef struct io_apic io_apic_t;

struct local_apic_address_override
{
    madt_entry_header_t header;
    uint16_t reserved;
    uint64_t local_apic_address;
}__attribute__ ((packed));
typedef struct local_apic_address_override local_apic_address_override_t;

struct processor_local_x2apic
{
    madt_entry_header_t header;
    uint16_t reserved;
    uint32_t x2apic_id;
    uint32_t flags;
    uint32_t acpi_processor_uid;
}__attribute__ ((packed));
typedef struct processor_local_x2apic processor_local_x2apic_t;


void* find_madt(rsdt_t *root_sdt)
{
    uint32_t entries = (root_sdt->header.length - sizeof(root_sdt->header)) / 4;
    
    uint32_t i = 0;
    while(i < entries)
    {
        madt_t *madt = (madt_t *) root_sdt->pointer_to_other_sdt[i];
        printf("%s\n", madt->header.signature);
        char* signature_ptr = (char*)(madt->header.signature);
        if (*signature_ptr == 'A' &&
            *(signature_ptr + 1) == 'P' &&
            *(signature_ptr + 2) == 'I' &&
            *(signature_ptr + 3) == 'C')
            return (void *) madt;
        ++i;
    }
 
    return 0;
}

void about_apic()
{
    uint8_t is_ebda_successed;
    uint8_t is_mem_reg_successed;
    uint16_t* ebda_addr = (uint16_t*)MAGIC_POINTER;
    uint32_t ebda_value = *ebda_addr;
    char* ebda_ptr = (char*)ebda_value;
    printf("%x\n", *ebda_ptr);
    char* end_addr = ebda_ptr + EBDA_FIND_LENGTH;
    while(ebda_ptr < end_addr)
    {
        if(*ebda_ptr == 'R'){
            printf("FOUND!!!\n");
        }
        ++ebda_ptr;
    }

    rsdp_descriptor_t* desc_ptr = 0;
    char* mem_ptr = (char*)MEM_REG_BEGIN;
    char* mem_end = (char*)MEM_REG_END;
    while(mem_ptr < mem_end)
    {
        if( *mem_ptr       == 'R' && 
            *(mem_ptr + 1) == 'S' && 
            *(mem_ptr + 2) == 'D' && 
            *(mem_ptr + 3) == ' ' && 
            *(mem_ptr + 4) == 'P' && 
            *(mem_ptr + 5) == 'T' && 
            *(mem_ptr + 6) == 'R' && 
            *(mem_ptr + 7) == ' ')
        {
            desc_ptr = (rsdp_descriptor_t*) mem_ptr;
            break;
        }
        ++mem_ptr;
    }

    if(desc_ptr == 0)
    {
        printf("descriptor not found :(\n");
    }
    else
    {
        printf("descriptor found :)\n");
        printf("Rsdt address = 0x%x\n", desc_ptr->rsdt_address);
        rsdt_t* rsdt_ptr = (rsdt_t*) desc_ptr->rsdt_address;
        uint32_t entries_count = (rsdt_ptr->header.length - sizeof(rsdt_ptr->header)) / 4;
        printf("%s\n", rsdt_ptr->header.signature);
        printf("Entries count = %d\n", entries_count);
        uint32_t i = 0;
        printf("signature =  %s\n", rsdt_ptr->header.signature);    
        
        madt_t* madt_ptr = find_madt(rsdt_ptr);

        if(madt_ptr->flags & 0x1) 
        {
            printf("PC/AT dual PIC supported.\n");
        }
        else
        {
            printf("PC/AT dual PIC not supported.\n");
        }

        uint32_t local_apic_address = madt_ptr->local_controller_address;
        printf("local apic address = %x\n", local_apic_address);

        if((void*)madt_ptr != 0)
        {
            printf("%s\n", "APIC found");
            printf("length = %d\n", madt_ptr->header.length);
        }


    }
}

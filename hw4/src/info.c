#include "includes/info.h"

#define CMDLINE_IS_VALID  0x04
#define MMAP_IS_VALID     0x40
#define MAGIC_POINTER     0x40E
#define EBDA_FIND_LENGTH  1024
#define MEM_REG_BEGIN     0x000E0000
#define MEM_REG_END       0x000FFFFF

#define PROC_LOCAL_APIC_TYPE          0x0
#define IO_APIC_TYPE                  0x1
#define LOCAL_APIC_ADDR_OVERRIDE_TYPE 0x5
#define PROC_LOCAL_X2_APIC_TYPE       0x9

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
    uint8_t io_apic_id;
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
        char* signature_ptr = (char*)(madt->header.signature);
        if (*signature_ptr       == 'A' &&
            *(signature_ptr + 1) == 'P' &&
            *(signature_ptr + 2) == 'I' &&
            *(signature_ptr + 3) == 'C')
            return (void *) madt;
        ++i;
    }
 
    return 0;
}

void* find_rsdt_substring(char* from, char* to)
{
    char* mem_ptr = from;
    while(mem_ptr + 7 < to)
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
            return mem_ptr;
        }
        ++mem_ptr;
    }

    return 0;
}

void* find_rsd_ptr()
{
    uint16_t* ebda_addr = (uint16_t*)MAGIC_POINTER;
    uint32_t ebda_value = (uint32_t)(*ebda_addr);

    char* ebda_ptr = (char*)ebda_value;
    char* end_addr = ebda_ptr + EBDA_FIND_LENGTH;

    // Найдем только один адрес, соответствующий первой найденной строке "RSD PTR "
    void* result = find_rsdt_substring(ebda_ptr, end_addr);
    if(result == (void*) 0){
        result = find_rsdt_substring((char*)MEM_REG_BEGIN, (char*)MEM_REG_END);
    }

    return result;
}

void print_madt_info(madt_t* madt)
{
    if(madt->flags & 0x1) 
    {
        printf("PC/AT dual PIC supported.\n");
    }
    else
    {
        printf("PC/AT dual PIC not supported.\n");
    }

    uint32_t local_apic_address = madt->local_controller_address;

    void* end_of_madt = (void*) madt + madt->header.length;
    // printf("madt: 0x%x - 0x%x (%d bytes)\n", (uint32_t)madt, end_of_madt, madt->header.length);

    madt_entry_header_t* entry_ptr = (void*) madt + sizeof(*madt);
    // printf("type = %d, length = %d\n", entry_ptr->entry_type, entry_ptr->record_length);
    // entry_ptr += entry_ptr->record_length;
    proc_local_apic_t* local_apic;
    io_apic_t* io_apic;
    local_apic_address_override_t* new_local_address_ptr;
    processor_local_x2apic_t* x2_apic_ptr;

    while((void*) entry_ptr <= end_of_madt)
    {
        // printf("type = %d, length = %d\n", entry_ptr->entry_type, entry_ptr->record_length);
        
        switch(entry_ptr->entry_type)
        {
            case PROC_LOCAL_APIC_TYPE:
                // printf("Processor Local APIC found\n");
                local_apic = (proc_local_apic_t*)entry_ptr;
                printf("Local APIC %d\n", local_apic->apic_id);
                break;
            case IO_APIC_TYPE:
                // printf("I/O APIC found\n");
                io_apic = (io_apic_t*)entry_ptr;
                printf("IOAPIC %d at 0x%x IRQs from %d\n", io_apic->io_apic_id, io_apic->io_apic_address, io_apic->global_system_interrupt_base);
                break;
            case LOCAL_APIC_ADDR_OVERRIDE_TYPE:
                // printf("Local APIC Address Override found\n");
                new_local_address_ptr = (local_apic_address_override_t*) entry_ptr;
                local_apic_address = new_local_address_ptr->local_apic_address;
                break;
            case PROC_LOCAL_X2_APIC_TYPE:
                // printf("Processor Local x2APIC found\n");
                x2_apic_ptr = (processor_local_x2apic_t*) entry_ptr;
                printf("Local x2APIC %d\n", x2_apic_ptr->x2apic_id);
                break;
        }

        entry_ptr = (madt_t*)((void*)entry_ptr + entry_ptr->record_length);
    }

    printf("Local APICs accessible at 0x%x\n", local_apic_address);
}

void about_apic()
{
    rsdp_descriptor_t* desc_ptr = (rsdp_descriptor_t*)find_rsd_ptr();

    if(desc_ptr == 0)
    {
        printf("descriptor not found :(\n");
        return;
    } 
    
    madt_t* madt_ptr = find_madt((rsdt_t*)(desc_ptr->rsdt_address));


    print_madt_info(madt_ptr);
}

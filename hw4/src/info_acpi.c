#include "includes/info_acpi.h"
#include "includes/int_types.h"

// Заголовок, присутствующий во всех SDT
typedef struct sdt_header sdt_header_t;

// Структуры для получения MADT
typedef struct rsdp_descriptor rsdp_descriptor_t;
typedef struct rsdt rsdt_t;

// MADT
typedef struct madt madt_t;

// Записи MADT
typedef struct madt_entry_header madt_entry_header_t; // Заголовок. Присутствует во всех записях

// Структуры для каждого типа записи.
typedef struct proc_local_apic proc_local_apic_t;
typedef struct io_apic io_apic_t;
typedef struct local_apic_address_override local_apic_address_override_t;
typedef struct processor_local_x2apic processor_local_x2apic_t;

struct rsdp_descriptor 
{
    char signature[8];       // Содержит "RSD PTR "
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdt_address;
}__attribute__ ((packed));

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
}__attribute__ ((packed));

struct rsdt
{
    sdt_header_t header;
    uint32_t pointer_to_other_sdt[]; 
}__attribute__ ((packed));

struct madt {
    sdt_header_t header;
    uint32_t local_controller_address;
    uint32_t flags;
}__attribute__ ((packed));

struct madt_entry_header
{
    uint8_t entry_type;
    uint8_t record_length;
}__attribute__ ((packed));

struct proc_local_apic
{
    madt_entry_header_t header;
    uint8_t  acpi_processor_id;
    uint8_t  apic_id;
    uint32_t flags;
}__attribute__ ((packed));

struct io_apic
{
    madt_entry_header_t header;
    uint8_t io_apic_id;
    uint8_t reserved;
    uint32_t io_apic_address;
    uint32_t global_system_interrupt_base;
}__attribute__ ((packed));

struct local_apic_address_override
{
    madt_entry_header_t header;
    uint16_t reserved;
    uint64_t local_apic_address;
}__attribute__ ((packed));

struct processor_local_x2apic
{
    madt_entry_header_t header;
    uint16_t reserved;
    uint32_t x2apic_id;
    uint32_t flags;
    uint32_t acpi_processor_uid;
}__attribute__ ((packed));


void* find_madt(rsdt_t *root_sdt)
{
    uint32_t entries = (root_sdt->header.length - sizeof(root_sdt->header)) / 4;
    
    uint32_t i = 0;
    while(i < entries)
    {
        madt_t* madt = (madt_t*) root_sdt->pointer_to_other_sdt[i];
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
    if(result == (void*) 0)
    {
        result = find_rsdt_substring((char*)MEM_REG_BEGIN, (char*)MEM_REG_END);
    }

    return result;
}

void print_madt_info(madt_t* madt)
{
    if(madt->flags & PC_AT_DUAL_SUPPORTED_FLAG) 
    {
        printf("PC/AT dual PIC supported.\n");
    }
    else
    {
        printf("PC/AT dual PIC not supported.\n");
    }

    // По умолчанию этот адрес берем из madt
    uint32_t local_apic_address = madt->local_controller_address;

    proc_local_apic_t* local_apic;
    io_apic_t* io_apic;
    local_apic_address_override_t* new_local_address_ptr;
    processor_local_x2apic_t* x2_apic_ptr;

    madt_entry_header_t* entry_ptr = (void*) madt + sizeof(*madt);
    void* end_of_madt = (void*) madt + madt->header.length;
    while((void*) entry_ptr <= end_of_madt)
    {
        switch(entry_ptr->entry_type)
        {
            case PROC_LOCAL_APIC_TYPE:
                local_apic = (proc_local_apic_t*)entry_ptr;
                printf("Local APIC %d\n", local_apic->apic_id);
                break;
            case IO_APIC_TYPE:
                io_apic = (io_apic_t*)entry_ptr;
                printf("IOAPIC %d at 0x%x IRQs from %d\n", 
                    io_apic->io_apic_id, 
                    io_apic->io_apic_address, 
                    io_apic->global_system_interrupt_base);
                break;
            case LOCAL_APIC_ADDR_OVERRIDE_TYPE:
                new_local_address_ptr = (local_apic_address_override_t*) entry_ptr;
                local_apic_address = new_local_address_ptr->local_apic_address;
                break;
            case PROC_LOCAL_X2_APIC_TYPE:
                x2_apic_ptr = (processor_local_x2apic_t*) entry_ptr;
                printf("Local x2APIC %d\n", x2_apic_ptr->x2apic_id);
                break;
        }

        entry_ptr = (madt_entry_header_t*)((void*)entry_ptr + entry_ptr->record_length);
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

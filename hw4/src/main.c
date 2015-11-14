#include "includes/screen.h"
#include "includes/desc_tables.h"
#include "includes/pic.h"
#include "includes/int_types.h"
#include "includes/info.h"
#include "includes/info_acpi.h"

void cmain(multiboot_info_t* mis)
{
    cls();
    
    init_tables();
    init_timer();

    about_cmdline(mis);
    about_mmap(mis);
    about_apic();

    // asm volatile ("sti");
    

    while(1);
}

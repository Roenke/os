#include "includes/desc_tables.h"
#include "includes/gdt.h"
#include "includes/idt.h"

void init_tables()
{
    init_gdt();
    init_idt();
}
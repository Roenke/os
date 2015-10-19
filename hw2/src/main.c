#include "includes/screen.h"
#include "includes/desc_tables.h"
#include "includes/pic.h"

void cmain(void)
{
    cls();
    
    init_tables();
    init_timer();

    asm volatile ("sti");

    while(1);
}

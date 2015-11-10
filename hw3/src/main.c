#include "includes/screen.h"
#include "includes/desc_tables.h"
#include "includes/pic.h"
#include "includes/serial_port.h"
#include "includes/task.h"
#include "includes/helpers.h"

char buf;
int is_ready_to_write = 0;

void read(void)
{
    while(1)
    {
        buf = read_serial();
        is_ready_to_write = 1;
    }
}

void write(void)
{
    while(1)
    {
        while(!is_ready_to_write);
        printf("%c", buf);
        is_ready_to_write = 0;
    }
}

void cmain(void)
{
    cls();
    
    init_tables();
    init_timer();
    init_serial();

    init_multithreading();
    add_task(read);
    add_task(write);

    sti();
    
    while(1);
}

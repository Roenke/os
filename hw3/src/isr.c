#include "includes/screen.h"
#include "includes/isr.h"
#include "includes/helpers.h"
#include "includes/pic.h"
#include "includes/task.h"

void isr_handler(state_t state)
{
    static int skipped;

    if(state.int_num != 32){
        printf ("Unhandled exception %d\n", state.int_num);
        asm volatile ("cli");
        asm volatile ("hlt");
    }

    send_eoi(0);

    next_task();
}

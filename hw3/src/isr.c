#include "includes/screen.h"
#include "includes/isr.h"
#include "includes/helpers.h"
#include "includes/pic.h"
#include "includes/task.h"
#include "includes/serial_port.h"

const unsigned int *ptraddr=(unsigned int *)0x00000400;
const int interrupts_before_switch = 2;

void isr_handler(state_t state)
{
    static int skipped = 0;
    if(state.int_num != 32)
    {
        printf ("Unhandled expection %d\n", state.int_num);
    }  
    else
    {
        printf("tick");
        next_task();
        send_eoi(0);
    }
}

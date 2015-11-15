#include "includes/screen.h"
#include "includes/isr.h"
#include "includes/helpers.h"
#include "includes/pic.h"


const int interrupts_before_tick = 20;
void isr_handler(state_t state)
{
    static int skipped = 0;
    if(state.int_num != 32)
    {
        printf ("Unhandled expection %d\n", state.int_num);
    }  
    else
    {
        ++skipped;
        if (skipped == interrupts_before_tick) 
        {
            // printf ("tick\n");
            skipped = 0;
        }

        send_eoi(0);
    }
}

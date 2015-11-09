#include "includes/screen.h"
#include "includes/isr.h"
#include "includes/helpers.h"
#include "includes/pic.h"
#include "includes/task.h"
#include "includes/serial_port.h"


void dumpRegs (state_t regs)
{
    printf ("ds = %d \n", regs.ds);
    printf ("edi = %d, esi = %d, ebp = %d, esp = %d\n", regs.edi, regs.esi, regs.ebp, regs.esp);
    printf ("ebx = %d, ecx = %d, eax = %d\n", regs.ebx, regs.edx, regs.ecx, regs.eax);
    printf ("intNumber = %d, errorCode = %d\n", regs.int_num, regs.error);
    printf ("eip = %d, cs = %d, eflags = %d, useresp = %d, ss = %d\n", regs.eip, regs.cs, regs.eflags, regs.useresp, regs.ss);
}

void isr_handler(state_t state)
{
    static int skipped;

    if(state.int_num != 32){
        printf ("Unhandled exception %d\n", state.int_num);
        asm volatile ("cli");
        asm volatile ("hlt");
    }

    send_eoi(0);

    // dumpRegs(state);
    next_task();
}

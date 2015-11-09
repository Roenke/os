#include "includes/task.h"
#include "includes/screen.h"
#include "includes/isr.h"
typedef void (*thread_function)(void);

extern state_t regs[];
extern int threads;
int stacks[10000];

void addNewThread(void (*fptr) (void) )
{
    regs[threads] = regs[0];
    regs[threads].eip = (int)fptr;
    regs[threads].eflags |= (1 << 9);   //мы хотим, чтобы прерывания были включены, иначе мы не сможем переключиться.
    regs[threads].esp = (int)&stacks[(threads + 1) * 1000] + sizeof (regs[threads]);
    threads++;
}


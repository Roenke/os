#include "includes/task.h"
#include "includes/screen.h"
#include "includes/isr.h"
#include "includes/helpers.h"


struct task
{
    context_t* regs;
    task_t *next;
};

task_t   main_thread;
task_t*  current_task;

uint32_t task_count = 0;
int      stacks[1000 * 1024];
task_t   tasks[1001];

void init_multithreading()
{
    current_task = &main_thread;
    current_task->next = current_task;
}

void create_task(task_t *task, thread_function function, uint32_t* stack) 
{
    stack -= sizeof (task->regs);
    
    task->regs = (context_t*) stack;
    task->regs->edi = 0;
    task->regs->esi = 0;
    task->regs->ebx = 0;
    task->regs->ebp = 0;
    task->regs->eflags = 0x200;
    task->regs->eip = (uint32_t) function;

    ++task_count;

    if (task_count == 1)
    {
        task->next = task;
        current_task->next = task;
    } 
    else 
    {
        task->next = current_task->next;
        current_task->next->next = task;
    }
}

void add_task(thread_function fun)
{
    uint8_t is_enabled = readeflags() & 0x200;
    cli();
    create_task(&tasks[task_count], fun, &stacks[1024 * (task_count + 1)]);
    
    if(is_enabled)
    {
        sti();
    }
}

void next_task()
{
    uint8_t is_enabled = readeflags() & 0x200;
    cli();
    
    if (task_count <= 1)
    {
        return;
    }

    task_t *previous_task = current_task;
    current_task = current_task->next;
    switch_task(&(previous_task->regs), current_task->regs);

    if(is_enabled)
    {
        sti();
    }
}

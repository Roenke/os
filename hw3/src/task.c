#include "includes/task.h"
#include "includes/screen.h"
#include "includes/isr.h"
typedef void (*thread_function)(void);

#define FL_IF           0x00000200

const int max_tasks = 1000;
int stacks[1024*1024];

struct task{
    struct context* regs;
};

int task_count = 1;
struct task tasks[1000];
int current_task_ptr = 0;

void add_task(thread_function fun)
{
    ++task_count;

    printf("Add task %d\n", task_count);
    context_t* new_context = (context_t *)(&stacks[task_count * 1024]);
    // new_context->eflags = FL_IF;
    new_context->edi = 0;
    new_context->esi = 0;
    new_context->ebx = 0;
    new_context->ebp = 0;
    new_context->eip = (uint32_t)fun;
    
    tasks[task_count - 1].regs = new_context;
}

void next_task()
{
    int prev_value = current_task_ptr;
    current_task_ptr = (prev_value + 1) % task_count;
    printf("In next task\n");
    printf("Switch task %d, with task %d.\n", prev_value, current_task_ptr);   
    switch_task(&(tasks[prev_value].regs), tasks[current_task_ptr].regs);
    // printf("In next task\n");
}
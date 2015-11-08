#include "includes/task.h"
#include "includes/screen.h"

typedef void (*thread_function)(void);

const int max_tasks = 1000;
int stacks[1024*1024];

struct task{
    thread_function exec_functoin;
    struct context* regs;
    int* stack_ptr;
};

int task_count = 1;
struct task tasks[1000];
int current_task_ptr = 0;

void end_of_task_handler()
{
    while(1){ continue; }
}

void add_task(thread_function fun)
{
    ++task_count;

    printf("Add task %d", task_count);
    struct context* new_context = (struct context*)(&stacks[task_count * 1024]);
    
    new_context->edi = 0;
    new_context->esi = 0;
    new_context->ebx = 0;
    new_context->ebp = 0;
    new_context->eip = fun;

    tasks[task_count - 1].regs = new_context;
}

void next_task()
{
    int prev_value = current_task_ptr;
    current_task_ptr = (prev_value + 1) % task_count;
    printf("In next task\n");
    switch_task(&(tasks[prev_value].regs), tasks[current_task_ptr].regs);
    printf("In next task\n");
}


#include "includes/task.h"
#include "includes/screen.h"
#include "includes/isr.h"
#define FL_IF           0x00000200

typedef void (*thread_function)(void);
typedef struct context context_t;
typedef struct task task_t;

task_t main_thread;
task_t* current_task;

uint32_t task_count = 0;

const int max_tasks = 1000;
int stacks[1024*1024];

struct task{
    struct context* regs;
    task_t *next;
};

task_t tasks[1000];

void initialize()
{
    current_task = &main_thread;
    current_task->next = current_task;
    task_count = 0;
}

void create_task(task_t *task, thread_function function, uint32_t* stack) {
    stack -= sizeof (task->regs);
    task->regs = (context_t*) stack;
    task->regs->edi = 0;
    task->regs->esi = 0;
    task->regs->ebx = 0;
    task->regs->ebp = 0;
    task->regs->eip = (uint32_t) function;
    ++task_count;
    if (task_count == 1){
        task->next = task;
        current_task->next = task;
    } else {
        task->next = current_task->next;
        current_task->next->next = task;
    }
}


void add_task(thread_function fun)
{
    create_task(&tasks[task_count], fun, &stacks[1024 * (task_count + 1)]);
    printf("Task created. Total count = %d", task_count);
    // ++task_count;

    // printf("Add task %d\n", task_count);
    // context_t* new_context = (context_t *)(&stacks[task_count * 1024] - sizeof(context_t));
    // // new_context->eflags = FL_IF;
    // new_context->edi = 0;
    // new_context->esi = 0;
    // new_context->ebx = 0;
    // new_context->ebp = 0;
    // new_context->eip = (uint32_t)fun;
    
    // tasks[task_count - 1].regs = new_context;
    // next_task();
    // asm volatile ("sti");
}

void next_task()
{
    if (task_count > 1){
        task_t *previous_task = current_task;
        current_task = current_task->next;
        switch_task(&(previous_task->regs), current_task->regs);
    }
    // int prev_value = current_task_ptr;
    // current_task_ptr = (prev_value + 1) % task_count;
    // // printf("In next task\n");
    // // printf("Switch task %d, with task %d.\n", prev_value, current_task_ptr);   
    // switch_task(&(tasks[prev_value].regs), tasks[current_task_ptr].regs);
    // // printf("In next task\n");
}
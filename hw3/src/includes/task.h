#ifndef TASK_H
#define TASK_H
#include "int_types.h"
#include "isr.h"

struct context {
  uint32_t eflags;
  uint32_t edi;
  uint32_t esi;
  uint32_t ebx;
  uint32_t ebp;
  uint32_t eip;
};

typedef struct context context_t;
typedef struct task task_t;
typedef void   (*thread_function)(void);

void init_multithreading();
void add_task(thread_function fun);
void next_task(); 

#endif /* TASK_H */

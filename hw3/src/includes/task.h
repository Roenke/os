#ifndef TASK_H
#define TASK_H
#include "int_types.h"

struct context {
  uint32_t edi;
  uint32_t esi;
  uint32_t ebx;
  uint32_t ebp;
  uint32_t eip;
};

extern void switch_task(struct context **, struct context *); 

#endif /* TASK_H */
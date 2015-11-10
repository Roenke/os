#ifndef ISR_H
#define ISR_H
#include "int_types.h"

struct state
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_num, error;
    uint32_t eip, cs, eflags, useresp, ss;
}__attribute__((packed));

typedef struct state state_t;

#endif /* ISR_H */
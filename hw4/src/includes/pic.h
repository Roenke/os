#ifndef PIC_H
#define PIC_H
#include "int_types.h"

void init_timer();
void send_eoi(uint8_t irq);

#endif
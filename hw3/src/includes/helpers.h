#ifndef HELPERS_H
#define HELPERS_H
#include "int_types.h"

void outb(uint8_t val, uint16_t port);
void memset(uint8_t *dest, uint8_t val, uint32_t len);
uint8_t inb(uint16_t port);

#endif

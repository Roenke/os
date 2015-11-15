#include "includes/helpers.h"

void outb(uint8_t value, uint16_t port_num)
{
    asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(port_num) );
}

void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
    for ( ; len != 0; len--) *dest++ = val;
}

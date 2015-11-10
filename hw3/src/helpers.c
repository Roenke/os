#include "includes/helpers.h"

void outb(uint8_t value, uint16_t port_num)
{
    asm volatile ( "outb %0, %1" : : "a"(value), "Nd"(port_num) );
}

uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}


void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
    for ( ; len != 0; len--) *dest++ = val;
}

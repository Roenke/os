#include "includes/serial_port.h"
#include "includes/int_types.h"
#include "includes/helpers.h"


const uint32_t port = 0x3f8; // COM1 port

void init_serial()
{
    outb(0x00, port + 1);    // Disable all interrupts
    outb(0x80, port + 3);    // Enable DLAB (set baud rate divisor)
    outb(0x03, port + 0);    // Set divisor to 3 (lo byte) 38400 baud
    outb(0x00, port + 1);    //                  (hi byte)
    outb(0x03, port + 3);    // 8 bits, no parity, one stop bit
    outb(0xC7, port + 2);    // Enable FIFO, clear them, with 14-byte threshold
    outb(0x0B, port + 4);    // IRQs enabled, RTS/DSR set
}

int serial_received()
{
    return inb(port + 5) & 1;
}

char read_serial()
{
    while (serial_received() == 0);

    return inb(port);
}

int is_transmit_empty()
{
    return inb(port + 5) & 0x20;
}

void write_serial(char a)
{
    while (is_transmit_empty() == 0);
 
    outb(a, port);
}

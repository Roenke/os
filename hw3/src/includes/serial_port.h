#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

// TODO: наверное не все функции публичные
void init_serial();
int serial_received();
char read_serial();
int is_transmit_empty();
void write_serial(char a);

#endif
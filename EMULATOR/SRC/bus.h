#ifndef BUS_H
#define BUS_H

#include <stdint.h>

void bus_init();

void bus_close();

uint8_t bus_read();

void bus_write(uint8_t byte);

#endif
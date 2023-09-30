#ifndef BUS_H
#define BUS_H

#include <stdint.h>

void bus_init(void);

void bus_close(void);

uint8_t bus_read(void);

void bus_write(uint8_t byte);

#endif
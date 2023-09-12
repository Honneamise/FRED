#ifndef MEMORY_H
#define MEMORY_H

#define CHARSET_BASE 0x07D0
#define SYSTEM_BASE  0x0FD0
#define PROGRAM_BASE 0x1000

#include <stdint.h>

void memory_init();

void memory_close();

uint8_t *memory_get_buffer();

uint8_t memory_read(uint16_t address);

void memory_write(uint16_t address, uint8_t byte);


#endif
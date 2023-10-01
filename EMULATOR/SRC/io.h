#ifndef IO_H
#define IO_H

#include <stdint.h>

#define IO_INP 0
#define IO_OUT 1

typedef void(*IO_func)(void);

typedef enum IO_PARAM
{
    IE,
	Q,
    EF1,
    EF2,
    EF3,
    EF4

} IO_PARAM;

void io_dump(void);

void io_init(void);

void io_close(void);

void io_set(IO_PARAM param, uint8_t val);

uint8_t io_get(IO_PARAM param);

void io_add_device(uint8_t id, IO_func input, IO_func output);

void io_activate_device(uint8_t id, uint8_t io_op);

#endif
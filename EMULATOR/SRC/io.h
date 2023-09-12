#ifndef IO_H
#define IO_H

#include "device.h"
#include <stdint.h>

typedef enum IO_PARAM
{
    IE,
	Q,
    EF1,
    EF2,
    EF3,
    EF4

} IO_PARAM;

void io_dump();

void io_init();

void io_close();

void io_set(IO_PARAM param, uint8_t val);

uint8_t io_get(IO_PARAM param);

#endif
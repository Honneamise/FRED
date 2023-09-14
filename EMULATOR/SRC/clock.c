#include "bus.h"
#include <stdint.h>

static uint8_t clock = 0;

void clock_update()
{
    clock++;

    if(clock>=60) { clock=0; }
}

void clock_func(void *data)
{
    uint8_t val = clock;

    bus_write(val);
}
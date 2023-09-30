#include "bus.h"
#include <stdint.h>

static uint8_t clock = 0;

void clock_update(void)
{
    clock++;

    if(clock>=60) { clock=0; }
}

void clock_func(void)
{
    uint8_t val = clock;

    bus_write(val);
}
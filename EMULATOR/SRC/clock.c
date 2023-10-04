#include "bus.h"
#include <stdint.h>

static uint8_t clock = 0;

void clock_update(void)
{
    clock++;

    if(clock>=60) { clock=0; }
}

void clock_input_func(void)
{
    bus_write(clock);
}
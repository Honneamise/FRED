#include "funcs.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct BUS
{
    uint8_t data;

} BUS;

static BUS *bus = NULL;

/**********/
void bus_init() 
{
    bus = alloc(1, sizeof(BUS));
}

/**********/
void bus_close() 
{
    free(bus);
}

/**********/
uint8_t bus_read() 
{
    uint8_t byte = 0;

    byte = bus->data;

    return byte;
}

/**********/
void bus_write(uint8_t byte) 
{
    bus->data = byte;
}


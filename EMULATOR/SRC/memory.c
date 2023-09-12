#include "funcs.h"
#include <stdint.h>
#include <stdlib.h>

#define MEMORY_SIZE 64000

typedef struct MEMORY
{
    uint8_t *data;

} MEMORY;

static MEMORY *memory = NULL;

/**********/
void memory_init() 
{
    memory = alloc(1, sizeof(MEMORY));
    
    memory->data = alloc(MEMORY_SIZE, sizeof(uint8_t));
}

/**********/
void memory_close() 
{
    free(memory->data);

    free(memory);
}

/**********/
uint8_t *memory_get_buffer()
{
    return memory->data;
}

/**********/
uint8_t memory_read(uint16_t address) 
{
    uint8_t byte = 0;

    byte = memory->data[address];

    return byte;
}

/**********/
void memory_write(uint16_t address, uint8_t byte) 
{
    memory->data[address] = byte;
}


#include "funcs.h"
#include "io.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_DEVICES 7

typedef struct Device
{
    IO_func input;
    IO_func output;

} Device;

typedef struct IO
{
	uint8_t IE;//1bit interrupt enable

	uint8_t Q;//1bit line

	uint8_t EF1;//1bit lines
    uint8_t EF2;//1bit lines
    uint8_t EF3;//1bit lines
    uint8_t EF4;//1bit lines

    Device devices[NUM_DEVICES];

    //uint8_t N_LINES;// 3 x 1 bit IO lines

    //uint8_t INTR;//1bit lines
    //uint8_t DMA_IN;//1bit lines
    //uint8_t DMA_OUT;//1bit lines

} IO;

static IO *io = NULL;

/**********/
void io_dump(void)
{
    printf("IE: %d\n", (int)io->IE);

	printf("Q: %d\n", (int)io->Q);

	printf("EF1: %d\n", (int)io->EF1);
    printf("EF2: %d\n", (int)io->EF2);
    printf("EF3: %d\n", (int)io->EF3);
    printf("EF4: %d\n", (int)io->EF4);
}

/**********/
void io_init(void) 
{
    io = alloc(1, sizeof(IO));
}

/**********/
void io_close(void) 
{
    free(io);
}

/**********/
void io_set(IO_PARAM param, uint8_t val)
{
    switch(param)
    {
        case IE         : io->IE        = val; break;
	    case Q          : io->Q         = val; break;
        case EF1        : io->EF1       = val; break;
        case EF2        : io->EF2       = val; break;
        case EF3        : io->EF3       = val; break;
        case EF4        : io->EF4       = val; break;

        default:
            error("[%s][INVALID PARAM I/O SELECTED]", __func__);
        break;
    }

}

/**********/
uint8_t io_get(IO_PARAM param)
{
    uint8_t val = 0;

    switch(param)
    {
        case IE         : val = io->IE;       break;
	    case Q          : val = io->Q;        break;
        case EF1        : val = io->EF1;      break;
        case EF2        : val = io->EF2;      break;
        case EF3        : val = io->EF3;      break;
        case EF4        : val = io->EF4;      break;

        default:
            error("[%s][INVALID I/O PARAM SELECTED]", __func__);
        break;
    }

    return val;
}

/**********/
void io_add_device(uint8_t id, IO_func input, IO_func output)
{
    if(id<1 || id>7) { error("[%s][INVALID I/O DEVICE SELECTED]", __func__); }
    else
    {
        io->devices[id-1].input = input;

        io->devices[id-1].output = output;
    }
}

/**********/
void io_activate_device(uint8_t id, uint8_t io_op)
{
    if(id<1 || id>7) { error("[%s][INVALID I/O DEVICE SELECTED]", __func__); }
    else
    {
        if(io_op == IO_INP && io->devices[id-1].input) 
        { 
            io->devices[id-1].input();
        }
        else if (io_op == IO_OUT && io->devices[id-1].output) 
        {
            io->devices[id-1].output();
        }
        else 
        { 
            error("[%s][INVALID I/O DEVICE OPERATION OR FUNCTION SELECTED][ID:%d]", __func__, id); 
        }
    
    }
}


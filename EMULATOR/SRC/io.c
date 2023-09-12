#include "funcs.h"
#include "io.h"
#include "device.h"
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

typedef struct IO
{
	uint8_t IE;//1bit interrupt enable

	uint8_t Q;//1bit line

	uint8_t EF1;//1bit lines
    uint8_t EF2;//1bit lines
    uint8_t EF3;//1bit lines
    uint8_t EF4;//1bit lines

    //uint8_t N_LINES;// 3 x 1 bit IO lines

    //uint8_t INTR;//1bit lines
    //uint8_t DMA_IN;//1bit lines
    //uint8_t DMA_OUT;//1bit lines

    CRITICAL_SECTION mutex;

} IO;

static IO *io = NULL;

/**********/
void io_dump()
{
    printf("IE: %d\n", (int)io->IE);

	printf("Q: %d\n", (int)io->Q);

	printf("EF1: %d\n", (int)io->EF1);
    printf("EF2: %d\n", (int)io->EF2);
    printf("EF3: %d\n", (int)io->EF3);
    printf("EF4: %d\n", (int)io->EF4);
}

/**********/
void io_init() 
{
    io = alloc(1, sizeof(IO));

    InitializeCriticalSection(&io->mutex);
}

/**********/
void io_close() 
{
    free(io);
}

/**********/
void io_set(IO_PARAM param, uint8_t val)
{
    EnterCriticalSection(&io->mutex);

    switch(param)
    {
        case IE         : io->IE        = val; break;
	    case Q          : io->Q         = val; break;
        case EF1        : io->EF1       = val; break;
        case EF2        : io->EF2       = val; break;
        case EF3        : io->EF3       = val; break;
        case EF4        : io->EF4       = val; break;

        default:
            error("[%s][INVALID I/O SELECTED]", __func__);
        break;
    }

    LeaveCriticalSection(&io->mutex);
}

/**********/
uint8_t io_get(IO_PARAM param)
{
    EnterCriticalSection(&io->mutex);

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
            error("[%s][INVALID I/O SELECTED]", __func__);
        break;
    }

    LeaveCriticalSection(&io->mutex);

    return val;
}


#include "funcs.h"
#include "device.h"

#include <stdlib.h>

typedef struct DEVICE
{
    uint8_t id;

    Func func;

    struct DEVICE *next;

} DEVICE;

static DEVICE *devices = NULL;

/**********/
void devices_close() 
{
    DEVICE *d = devices;

    while(d)
    {
        devices = d->next;

        free(d);

        d = devices;
    }
}

/**********/
void device_add(uint8_t id, Func func)
{
    DEVICE *d = alloc(1, sizeof(DEVICE));

    d->id = id;
    d->func = func;
    d->next = devices;

    devices = d;
}

/**********/
void device_activate(uint8_t device_id)
{
    DEVICE *d = devices;

    while(d)
    {
        if(d->id == device_id) 
        {
            d->func(NULL);

            return;
        }

        d = d->next;
    }

    error("[%s][DEVICE ID NOT FOUND][%d]", __func__, device_id);
}
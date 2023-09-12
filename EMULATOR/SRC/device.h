#ifndef DEVICE_H
#define DEVICE_H

#include <stdint.h>

typedef void(*Func)(void *data);

void devices_close();

void device_add(uint8_t id, Func func);

void device_activate(uint8_t device_id);

#endif
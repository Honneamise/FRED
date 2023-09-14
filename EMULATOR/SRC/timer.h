#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

double timer_get();

void timer_sleep(int64_t usec);

#endif
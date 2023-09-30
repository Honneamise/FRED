#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

void system_init(char *charset_file, char *program_file);

void system_close(void);

void system_run(uint8_t mhz);

#endif

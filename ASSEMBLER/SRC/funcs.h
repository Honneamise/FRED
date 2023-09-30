#ifndef FUNCS_H
#define FUNCS_H

#include <stdint.h>

#define ARRAY_SIZE(x) ( sizeof((x)) / sizeof((x)[0]) )

void error(const char * format, ...);

void *alloc(size_t count, size_t size);

void load_buffer(char *file, uint8_t **buffer, size_t *size);

void save_buffer(char *file, uint8_t *buffer, size_t size);

uint8_t str_to_word(char *str, uint16_t *num);

#endif
#ifndef CONTEXT_H
#define CONTEXT_H

#include <stdint.h>

typedef struct Symbol
{
    char *name;

    uint16_t val;

    struct Symbol *next;

} Symbol;

typedef struct Context
{
    uint16_t byte_counter;

    Symbol *table;

} Context;


Context *context_init();

void context_close(Context *ctx);

void context_add_symbol(Context *ctx, char *name);

Symbol *context_get_symbol(Context *ctx, char *name);

#endif
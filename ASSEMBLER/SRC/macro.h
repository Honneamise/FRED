#ifndef MACRO_H
#define MACRO_H

#include "context.h"
#include "stream.h"

//functions prototype
typedef void(*Parser)(Context *ctx, Stream *stream);
typedef void(*Builder)(Context *ctx, Stream *in, Stream *out);

typedef struct Macro
{
    char *name;

    Parser parser;

    Builder builder;

} Macro;

void *macro_get(char *name);

void macro_parse(Context *ctx, Stream *stream, Macro *macro);

void macro_build(Context *ctx, Stream *in, Stream *out, Macro *macro);

#endif
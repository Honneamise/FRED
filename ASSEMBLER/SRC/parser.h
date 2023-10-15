#ifndef PARSER_H
#define PARSER_H

#include "context.h"
#include "stream.h"

#define COMMA ','
#define DOT '.'
#define SEMICOLON ';'
#define COLON ':'
#define QUOTE '"'

#define HIGH_BYTE '1'
#define LOW_BYTE '0'

void assembler_parse(Context *ctx, Stream *stream);

#endif
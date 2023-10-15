#ifndef ISA_H
#define ISA_H

#include "context.h"
#include "stream.h"

#include <stdint.h>

//IO refer to EF1 to EF4
typedef enum PARAM_TYPE
{
    NONE,
    IO,
    REGISTER,
    BYTE,
    BYTE_ADDRESS,
    WORD_ADDRESS
    
} PARAM_TYPE;

typedef struct Instruction
{
    char *name;
    uint8_t opcode;
    PARAM_TYPE param_type;
    uint8_t param_size;

} Instruction;
 
int8_t isa_get_register(char *reg);

Instruction *isa_get_instruction(char *name);

void isa_parse(Context *ctx, Stream *stream, Instruction *ins);

void isa_build(Context *ctx, Stream *in, Stream *out, Instruction *ins);

#endif


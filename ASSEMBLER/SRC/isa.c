#include "funcs.h"
#include "isa.h"

#include <stdlib.h>
#include <string.h>

static Instruction ISA[] = 
{
    { "IDL",  0x00, NONE,           0 },
    { "LDN",  0x00, REGISTER,       0 },
    { "INC",  0x10, REGISTER,       0 },
    { "DEC",  0x20, REGISTER,       0 },
    { "BR",   0x30, BYTE_ADDRESS,   1 },
    { "BQ",   0x31, BYTE_ADDRESS,   1 },
    { "BZ",   0x32, BYTE_ADDRESS,   1 },
    { "BDF",  0x33, BYTE_ADDRESS,   1 },
    { "B1",   0x34, BYTE_ADDRESS,   1 },
    { "B2",   0x35, BYTE_ADDRESS,   1 },
    { "B3",   0x36, BYTE_ADDRESS,   1 },
    { "B4",   0x37, BYTE_ADDRESS,   1 },
    { "SKP",  0x38, NONE,           0 },
    { "BNQ",  0x39, BYTE_ADDRESS,   1 },
    { "BNZ",  0x3A, BYTE_ADDRESS,   1 },
    { "BNF",  0x3B, BYTE_ADDRESS,   1 },
    { "BN1",  0x3C, BYTE_ADDRESS,   1 },
    { "BN2",  0x3D, BYTE_ADDRESS,   1 },
    { "BN3",  0x3E, BYTE_ADDRESS,   1 },
    { "BN4",  0x3F, BYTE_ADDRESS,   1 },
    { "LDA",  0x40, REGISTER,       0 },
    { "STR",  0x50, REGISTER,       0 },
    { "IRX",  0x60, NONE,           0 },
    { "OUT",  0x60, IO,             0 },
    { "INP",  0x68, IO,             0 },
    { "RET",  0x70, NONE,           0 },
    { "DIS",  0x71, NONE,           0 },
    { "LDXA", 0x72, NONE,           0 },
    { "STXD", 0x73, NONE,           0 },
    { "ADC",  0x74, NONE,           0 },
    { "SDB",  0x75, NONE,           0 },
    { "SHRC", 0x76, NONE,           0 },
    { "SMB",  0x77, NONE,           0 },
    { "SAV",  0x78, NONE,           0 },
    { "MARK", 0x79, NONE,           0 },
    { "REQ",  0x0A, NONE,           0 },
    { "SEQ",  0x7B, NONE,           0 },
    { "ADCI", 0x7C, BYTE,           1 },
    { "SDBI", 0x7D, BYTE,           1 },
    { "SHLC", 0x7E, NONE,           0 },
    { "SMBI", 0x7F, BYTE,           1 },
    { "GLO",  0x80, REGISTER,       0 },
    { "GHI",  0x90, REGISTER,       0 },
    { "PLO",  0xA0, REGISTER,       0 },
    { "PHI",  0xB0, REGISTER,       0},
    { "LBR",  0xC0, WORD_ADDRESS,   2 },
    { "LBQ",  0xC1, WORD_ADDRESS,   2 },
    { "LBZ",  0xC2, WORD_ADDRESS,   2 },
    { "LBDF", 0xC3, WORD_ADDRESS,   2 },
    { "NOP",  0xC4, NONE,           0 },
    { "LSNQ", 0xC5, NONE,           0 },
    { "LSNZ", 0xC6, NONE,           0 },
    { "LSNF", 0xC7, NONE,           0 },
    { "LSKP", 0xC8, NONE,           0 },
    { "LBNQ", 0xC9, WORD_ADDRESS,   2 },
    { "LBNZ", 0xCA, WORD_ADDRESS,   2 },
    { "LBNF", 0xCB, WORD_ADDRESS,   2 },
    { "LSIE", 0xCC, NONE,           0 },
    { "LSQ",  0xCD, NONE,           0 },
    { "LSZ",  0xCE, NONE,           0 },
    { "LSDF", 0xCF, NONE,           0 },
    { "SEP",  0xD0, REGISTER,       0 },
    { "SEX",  0xE0, REGISTER,       0 },
    { "LDX",  0xF0, NONE,           0 },
    { "OR",   0xF1, NONE,           0 },
    { "AND",  0xF2, NONE,           0 },
    { "XOR",  0xF3, NONE,           0 },
    { "ADD",  0xF4, NONE,           0 },
    { "SD",   0xF5, NONE,           0 },
    { "SHR",  0xF6, NONE,           0 },
    { "SM",   0xF7, NONE,           0 },
    { "LDI",  0xF8, BYTE,           1 },
    { "ORI",  0xF9, BYTE,           1 },
    { "ANI",  0xFA, BYTE,           1 },
    { "XRI",  0xFB, BYTE,           1 },
    { "ADI",  0xFC, BYTE,           1 },
    { "SDI",  0xFD, BYTE,           1 },
    { "SHL",  0xFE, NONE,           0 },
    { "SMI",  0xFF, BYTE,           1 },
};

static char *REGISTERS_ALIAS[] = 
{
    "",
    "",
    "SP",
    "PC",
    "CALL",
    "RETN",
    "LINK",
    "",
    "",
    "",
    "AR",
    "NR",
    "CR",
    "MA",
    "MQ",
    "AC"
};

/**********/
// return the index of the register, -1 if not alias
static int8_t isa_get_alias_register(char *reg)
{
    for(int8_t i=0;i<16;i++)
    {
        if(strcmp(reg, REGISTERS_ALIAS[i])==0) { return i; }
    }

    return -1;
}

/**********/
// return the index of the register, -1 on errors
int8_t isa_get_register(char *reg)
{
    //is an alias ?
    int8_t alias = isa_get_alias_register(reg);
    if(alias!=-1){ return alias; }

    //explicit register
    uint16_t val = 0;

    if(reg[0]!='R') { return -1; }

    if( !str_to_word(&reg[1], &val) ) { return -1; }

    if( val>15 ) { return -1; }
    
    return (int8_t)val;
}

/**********/
Instruction *isa_get_instruction(char *name)
{
    for(uint8_t i=0;i<ARRAY_SIZE(ISA);i++)
    {
        if(strcmp(name,ISA[i].name)==0) { return &ISA[i]; }
    }

    return NULL;
}

/**********/
void isa_parse(Context *ctx, Stream *stream, Instruction *ins)
{
    switch(ins->param_type)
    {
        case NONE: break;
        
        case IO:
        case REGISTER:
        case BYTE_ADDRESS:
        case WORD_ADDRESS:
            stream_skip_blanks(stream);

            if(!stream_skip_token(stream)) 
            { 
                error("[%s][LINE %d][MISSING PARAMETER]", __func__, stream->line);
            }
        break;

        case BYTE:
            stream_skip_blanks(stream);

            if(!stream_skip_token(stream)) 
            { 
                error("[%s][LINE %d][MISSING PARAMETER]", __func__, stream->line);
            }

            if( stream_expect(stream, BYTE_SEPARATOR) )
            {
                if(!stream_expect(stream, HIGH_BYTE) && !stream_expect(stream, LOW_BYTE))
                {
                    error("[%s][LINE %d][EXPECTED HI OR LOW NIBBLE]", __func__, stream->line);
                }
            }
        break;

        default: 
            error("[%s][LINE %d][INVALID PARAMETER TYPE]", __func__, stream->line);
        break;
    }

    ctx->byte_counter += 1 + ins->param_size;
}

/**********/
void isa_build(Context *ctx, Stream *in, Stream *out, Instruction *ins)
{
    char *token = NULL;
    Symbol *symbol = NULL;
    uint16_t val = 0;
            
    //create the opcode according to param types
    switch(ins->param_type)
    {
        //NONE
        case NONE:
            stream_write_byte(out, ins->opcode);
        break;
        
        //I/O
        case IO:
            stream_skip_blanks(in);

            token = stream_get_token(in);

            if(!str_to_word(token, &val) || val<1 || val>7)
            {
                error("[%s][LINE %d][REQUIRED I/O LINE PARAMETER 1-7][FOUND %d]", __func__, in->line, val);
            }
            
            stream_write_byte(out, ins->opcode + (uint8_t)val);

            free(token);
        break;

        //REGISTER
        case REGISTER:

            stream_skip_blanks(in);

            token = stream_get_token(in);

            int8_t reg = isa_get_register(token);

            //NOTE : LDN cannot use R0
            if( reg<0 || ( strcmp(token,"LDN")==0 && reg==0) )
            {
                error("[%s][LINE %d][INVALID REGISTER SELECTED][%s]", __func__, in->line, token);
            }

            stream_write_byte(out, ins->opcode + (uint8_t)reg);

            free(token);
        break;

        //BYTE ADDRESS
        case BYTE_ADDRESS://can be byte or symbol

            stream_skip_blanks(in);

            token = stream_get_token(in);

            symbol = context_get_symbol(ctx, token);

            if(symbol)//symbol ?
            {
                //BYTE ADDRESS IS NOT AN OFFSET !!! 
                //ensure symbol is in current page (high byte of address must be the same)
                if( (ctx->byte_counter & 0xFF00) != ( (size_t)symbol->val & 0xFF00) )
                {
                    uint16_t page_start = (ctx->byte_counter & 0xFF00);
                    uint16_t page_end = (ctx->byte_counter & 0xFF00) + 0xFF;
                    error("[%s][LINE %d][SYMBOL ADDRESS MUST BE IN CURRENT PAGE 0x%04X-0x%04X][FOUND 0x%04X]", 
                            __func__, in->line, page_start, page_end, symbol->val);
                }

                stream_write_byte(out, ins->opcode );
                stream_write_byte(out, (uint8_t)(symbol->val & 0x00FF));
            }
            else if( str_to_word(token, &val) && val<=0xFF)//byte ?
            {
                stream_write_byte(out, ins->opcode);
                stream_write_byte(out, (uint8_t)val);
            }
            else //error
            {
                error("[%s][LINE %d][REQUIRED BYTE OR SYMBOL PARAMETER]", __func__, in->line);
            }

            free(token);
        break;

        //WORD ADDRESS
        case WORD_ADDRESS://can be word or symbol
            stream_skip_blanks(in);

            token = stream_get_token(in);

            symbol = context_get_symbol(ctx, token);

            if(symbol)//symbol ?
            {
                stream_write_byte(out, ins->opcode);
                stream_write_word(out, symbol->val);
            }
            else if( str_to_word(token, &val) )//word ?
            {
                stream_write_byte(out, ins->opcode);
                stream_write_word(out, (uint16_t)val);
            }
            else //error
            {
                error("[%s][LINE %d][REQUIRED WORD OR SYMBOL PARAMETER]", __func__, in->line);
            }

            free(token);
        break;

        //BYTE
        case BYTE: //can be byte or symbol byte
            stream_skip_blanks(in);

            token = stream_get_token(in);

            symbol = context_get_symbol(ctx, token);

            if(symbol && stream_expect(in, BYTE_SEPARATOR))//symbol ?
            {
                stream_write_byte(out, ins->opcode);

                if(stream_expect(in, HIGH_BYTE))
                {
                    stream_write_byte(out, (uint8_t)(symbol->val >> 8) );
                }
                
                if(stream_expect(in, LOW_BYTE))
                {
                    stream_write_byte(out, (uint8_t)(symbol->val & 0x00FF));
                }
                
            }
            else if( str_to_word(token, &val) && val<=0xFF)//byte ?
            {
                stream_write_byte(out, ins->opcode);
                stream_write_byte(out, (uint8_t)val);    
            }
            else//error :-)
            {
                error("[%s][LINE %d][REQUIRED BYTE PARAMETER]", __func__, in->line);
            }            
        
            free(token);
        break;

        //SHOULD NEVER HAPPEN
        default: 
            error("[%s][LINE %d][INVALID PARAMETER TYPE]", __func__, in->line);
        break;
    }

    ctx->byte_counter += 1 + ins->param_size;
}
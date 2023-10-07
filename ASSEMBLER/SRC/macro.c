#include "macro.h"
#include "isa.h"
#include "funcs.h"
#include "parser.h"
#include "builder.h"

#include <stdlib.h>
#include <string.h>

/*******/
/* ORG */
/*******/
static void org_p(Context *ctx, Stream *stream)
{
    if(ctx->byte_counter != 0) 
    { 
        error("[%s][LINE %d][ENTRY POINT ALREADY SET]", __func__, stream->line);
    }

    stream_skip_blanks(stream);

    char *token = stream_get_token(stream);

    if(!token)
    {
        error("[%s][LINE %d][MISSING PARAMETER]", __func__, stream->line);
    }

    uint16_t val = 0;
    if( !str_to_word(token, &val) )
    {
        error("[%s][LINE %d][INVALID ENTRY POINT ADDRESS]", __func__, stream->line);
    }

    free(token);

    ctx->byte_counter = val;
}

/**********/
static void org_b(Context *ctx, Stream *in, Stream *out)
{
    (void)out;//unused

    stream_skip_blanks(in);

    char *token = stream_get_token(in);

    uint16_t val = 0;
    str_to_word(token, &val);

    free(token);

    ctx->byte_counter = val;
}

/********/
/* BYTE */
/**********/
static void byte_p(Context *ctx, Stream *stream)
{
    do
    {
        stream_skip_blanks(stream);

        if( !stream_skip_token(stream) )
        {
            error("[%s][LINE %d][EXPECTED BYTE]", __func__, stream->line);
        }
        
        if( stream_expect(stream, BYTE_SEPARATOR) )
        {
            if(!stream_expect(stream, HIGH_BYTE) && !stream_expect(stream, LOW_BYTE))
            {
                error("[%s][LINE %d][EXPECTED HI OR LOW NIBBLE]", __func__, stream->line);
            }
        }

        stream_skip_blanks(stream);

        ctx->byte_counter++;

    } while (stream_expect(stream, COMMA) );
}

/**********/
static void byte_b(Context *ctx, Stream *in, Stream *out)
{
    do
    {
        stream_skip_blanks(in);

        char *token = stream_get_token(in);

        uint16_t val = 0;

        Symbol *symbol = NULL;

        if(str_to_word(token, &val) && val<=0xFF)//val
        {
            stream_write_byte(out, (uint8_t)val);
        }
        else if( (symbol = context_get_symbol(ctx,token)) != NULL && stream_expect(in, BYTE_SEPARATOR))//label
        {
            if(stream_expect(in, HIGH_BYTE))
            {
                stream_write_byte(out, symbol->val >> 8 );
            }
                
            if(stream_expect(in, LOW_BYTE))
            {
                stream_write_byte(out, symbol->val & 0x00FF);
            }
        }
        else
        {
            error("[%s][LINE %d][EXPECTED BYTE OR LABEL NIBBLE]", __func__, in->line);
        }

        free(token);

        stream_skip_blanks(in);

        ctx->byte_counter++;

    } while (stream_expect(in, COMMA) );
}

/********/
/* WORD */
/********/
static void word_p(Context *ctx, Stream *stream)
{
    do
    {
        stream_skip_blanks(stream);

        if( !stream_skip_token(stream) )
        {
            error("[%s][LINE %d][EXPECTED WORD OR LABEL]", __func__, stream->line);
        }
        
        stream_skip_blanks(stream);

        ctx->byte_counter += 2;

    } while (stream_expect(stream, COMMA) );
}

/**********/
static void word_b(Context *ctx, Stream *in, Stream *out)
{
    do
    {
        stream_skip_blanks(in);

        char *token = stream_get_token(in);

        uint16_t val = 0;
        Symbol *symbol = NULL;

        if(str_to_word(token, &val))//value
        {
            stream_write_word(out, val);
        }
        else if ( (symbol = context_get_symbol(ctx, token)) != NULL )//symbol
        {
            stream_write_word(out, symbol->val);
        }
        else //error
        {
            error("[%s][LINE %d][EXPECTED WORD OR LABEL]", __func__, in->line);
        }

        free(token);

        stream_skip_blanks(in);

        ctx->byte_counter += 2;         

    } while (stream_expect(in, COMMA) );
}

/***********/
/* RESERVE */
/***********/
static void reserve_p(Context *ctx, Stream *stream)
{
    stream_skip_blanks(stream);

    char *token = stream_get_token(stream);

    if(!token)
    {
        error("[%s][LINE %d][MISSING PARAMETER]", __func__, stream->line);
    }

    uint16_t val = 0;

    if(!str_to_word(token, &val) )
    {
        error("[%s][LINE %d][INVALID PARAMETER SIZE]", __func__, stream->line);
    }

    free(token);

    ctx->byte_counter += val;
}

/**********/
static void reserve_b(Context *ctx, Stream *in, Stream *out)
{
    stream_skip_blanks(in);

    char *token = stream_get_token(in);

    uint16_t val = 0;

    str_to_word(token, &val);

    for(uint16_t i=0;i<val;i++)
    {
        stream_write_byte(out, 0x00);
    }

    free(token);

    ctx->byte_counter += val;
}

/**********/
/* STRING */
/**********/
static void string_p(Context *ctx, Stream *stream)
{
    stream_skip_blanks(stream);

    //string start
    if(!stream_expect(stream, STRING_DELIMITER))
    {
        error("[%s][LINE %d][EXPECTED STRING DELIMITER]", __func__, stream->line);
    }
    
    //string body
    uint8_t *start = &stream->buffer[stream->pos];

    stream_skip_until(stream, STRING_DELIMITER);

    uint8_t *end = &stream->buffer[stream->pos];

    ctx->byte_counter += (uint16_t)(end-start);

    //string end
    if(!stream_expect(stream, STRING_DELIMITER))
    {
        error("[%s][LINE %d][EXPECTED STRING DELIMITER]", __func__, stream->line);
    }
}

/**********/
static void string_b(Context *ctx, Stream *in, Stream *out)
{
    stream_skip_blanks(in);

    //string start
    stream_expect(in, STRING_DELIMITER);
    
    //string body
    uint8_t *start = &in->buffer[in->pos];

    stream_skip_until(in, STRING_DELIMITER);

    uint8_t *end = &in->buffer[in->pos];

    for(;start!=end;start++)
    {
        stream_write_byte(out, *start);
    }

    ctx->byte_counter += (uint16_t)(end-start);

    stream_expect(in, STRING_DELIMITER);
}

/***************/
/* INCLUDE_SRC */
/***************/
static void src_p(Context *ctx, Stream *stream)
{
    stream_skip_blanks(stream);

    //string start
    if(!stream_expect(stream, STRING_DELIMITER))
    {
        error("[%s][LINE %d][EXPECTED STRING DELIMITER]", __func__, stream->line);
    }

    uint8_t *start = &stream->buffer[stream->pos];

    stream_skip_until(stream, STRING_DELIMITER);

    uint8_t *end = &stream->buffer[stream->pos];

    //expect string end
    if(!stream_expect(stream, STRING_DELIMITER))
    {
        error("[%s][LINE %d][EXPECTED STRING DELIMITER]", __func__, stream->line);
    }

    //get file name
    char *file_name = alloc( (end-start)+1, sizeof(char));

    memcpy(file_name, start, (end-start));

    //get the file as stream
    Stream *s = stream_init(file_name);

    //parse the file
    assembler_parse(ctx, s);

    free(file_name);

    stream_close(s);
}

/**********/
static void src_b(Context *ctx, Stream *in, Stream *out)
{
    stream_skip_blanks(in);

    //string start
    stream_expect(in, STRING_DELIMITER);

    uint8_t *start = &in->buffer[in->pos];

    stream_skip_until(in, STRING_DELIMITER);

    uint8_t *end = &in->buffer[in->pos];

    //expect string end
    stream_expect(in, STRING_DELIMITER);

    //get file name
    char *file_name = alloc( (end-start)+1, sizeof(char));

    memcpy(file_name, start, (end-start));

    //parse the include file
    Stream *s = stream_init(file_name);

    assembler_build(ctx, s, out);

    free(file_name);

    stream_close(s);
}

/***************/
/* INCLUDE_BIN */
/***************/
static void bin_p(Context *ctx, Stream *stream)
{
    stream_skip_blanks(stream);

    //string start
    if(!stream_expect(stream, STRING_DELIMITER))
    {
        error("[%s][LINE %d][EXPECTED STRING DELIMITER]", __func__, stream->line);
    }

    uint8_t *start = &stream->buffer[stream->pos];

    stream_skip_until(stream, STRING_DELIMITER);

    uint8_t *end = &stream->buffer[stream->pos];

    //expect string end
    if(!stream_expect(stream, STRING_DELIMITER))
    {
        error("[%s][LINE %d][EXPECTED STRING DELIMITER]", __func__, stream->line);
    }

    //get file name
    char *file_name = alloc( (end-start)+1, sizeof(char));

    memcpy(file_name, start, (end-start));

    //get the file as stream
    Stream *_stream = stream_init(file_name);

    //adjust the byte counter
    ctx->byte_counter += _stream->buffer_size;

    free(file_name);

    stream_close(_stream);
}

/**********/
static void bin_b(Context *ctx, Stream *in, Stream *out)
{
    stream_skip_blanks(in);

    //string start
    stream_expect(in, STRING_DELIMITER);

    uint8_t *start = &in->buffer[in->pos];

    stream_skip_until(in, STRING_DELIMITER);

    uint8_t *end = &in->buffer[in->pos];

    //expect string end
    stream_expect(in, STRING_DELIMITER);

    //get file name
    char *file_name = alloc( (end-start)+1, sizeof(char));

    memcpy(file_name, start, (end-start));

    //load the binary file
    Stream *s = stream_init(file_name);

    //copy it to output
    memcpy(&out->buffer[out->pos], s->buffer, s->buffer_size);

    //adjust the byte counter AND buffer position
    out->pos +=  s->buffer_size;
    ctx->byte_counter += s->buffer_size;

    free(file_name);

    stream_close(s);
}

/********/
/* LOAD */
/********/
static void load_p(Context *ctx, Stream *stream)
{
    stream_skip_blanks(stream);

    if(!stream_skip_token(stream))
    {
        error("[%s][LINE %d][MISSING DESTINATION PARAMETER]", __func__, stream->line);
    } 

    stream_skip_blanks(stream);

    if(!stream_expect(stream, COMMA))
    {
        error("[%s][LINE %d][EXPECTED COMMA]", __func__, stream->line);
    }

    stream_skip_blanks(stream);

    char *token = stream_get_token(stream);

    if(!token)
    {
        error("[%s][LINE %d][MISSING SOURCE PARAMETER]", __func__, stream->line);
    } 

    if(isa_get_register(token)!=-1)
    {
        /* REG-REG = GHI + PHI + GLO + PLO = 4 */
        ctx->byte_counter += 4;
    }
    else
    {
        /* REG-VAL or REG-LABEL = LDI + BYTE + PHI + LDI + BYTE + PLO = 6 */
        ctx->byte_counter += 6;
    }
  
    free(token);
}

/**********/
static void load_b(Context *ctx, Stream *in, Stream *out)
{
    stream_skip_blanks(in);

    char *reg_str = stream_get_token(in);
    int8_t reg_val = isa_get_register(reg_str);

    if( reg_val == -1 )
    {
        error("[%s][LINE %d][INVALID REGISTER SELECTED][%s]", __func__, in->line, reg_str);
    }

    stream_skip_blanks(in);
    stream_expect(in, COMMA);
    stream_skip_blanks(in);

    char *param_str = stream_get_token(in);
    int8_t param_8 = -1;
    uint16_t param_16 = 0;
    Symbol *param_symbol = NULL;

    if( (param_8 = isa_get_register(param_str)) != -1 ) /* REG-REG */
    {
        stream_write_byte(out, 0x90 + param_8); //GHI
        stream_write_byte(out, 0xB0 + reg_val); //PHI
        stream_write_byte(out, 0x80 + param_8); //GLO
        stream_write_byte(out, 0xA0 + reg_val); //PLO

        ctx->byte_counter += 4;
    }
    else if(str_to_word(param_str, &param_16)) /* REG-VAL */
    {
        stream_write_byte(out, 0xF8);               //LDI
        stream_write_byte(out, param_16 >> 8);      //BYTE
        stream_write_byte(out, 0xB0 + reg_val);     //PHI
        stream_write_byte(out, 0xF8);               //LDI
        stream_write_byte(out, param_16 & 0x00FF);  //BYTE
        stream_write_byte(out, 0xA0 + reg_val);     //PLO   

        ctx->byte_counter += 6;
    }
    else if( (param_symbol = context_get_symbol(ctx, param_str)) != NULL) /* REG-SYMBOL */
    {
        stream_write_byte(out, 0xF8);                       //LDI
        stream_write_byte(out, param_symbol->val >> 8);     //BYTE
        stream_write_byte(out, 0xB0 + reg_val);             //PHI
        stream_write_byte(out, 0xF8);                       //LDI
        stream_write_byte(out, param_symbol->val & 0x00FF); //BYTE
        stream_write_byte(out, 0xA0 + reg_val);             //PLO

        ctx->byte_counter += 6;
    }
    else
    {
        error("[%s][LINE %d][INVALID PARAMETER][%s]", __func__, in->line, param_str);
    }

    free(reg_str);
    free(param_str);
}


/*********/
/* MACRO */
/*********/
static Macro MACROS[] = 
{
    {"ORG", org_p, org_b},
    {"BYTE", byte_p, byte_b},
    {"WORD", word_p, word_b},
    {"RESERVE", reserve_p, reserve_b},
    {"STRING", string_p, string_b},
    {"INCLUDE_SRC", src_p, src_b},
    {"INCLUDE_BIN", bin_p, bin_b},
    {"LOAD", load_p, load_b}
};

/**********/
void *macro_get(char *name)
{
    for(uint8_t i=0;i<ARRAY_SIZE(MACROS);i++)
    {
        if( strcmp(name, MACROS[i].name)==0) { return &MACROS[i]; }
    }

    return NULL;
}

/**********/
void macro_parse(Context *ctx, Stream *stream, Macro *macro)
{
    macro->parser(ctx, stream);
}

/**********/
void macro_build(Context *ctx, Stream *in, Stream *out, Macro *macro)
{
    macro->builder(ctx, in, out);
}
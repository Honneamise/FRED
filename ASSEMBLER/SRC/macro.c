#include "macro.h"
#include "funcs.h"
#include "parser.h"
#include "builder.h"

#include <stdlib.h>
#include <string.h>

#define SEQUENCE_SEPARATOR ','
#define STRING_DELIMITER '"'

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

    ctx->byte_counter = (uint16_t)val;
}

/**********/
static void org_b(Context *ctx, Stream *in, Stream *out)
{
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
    stream_skip_blanks(stream);

    do
    {
        if( !stream_skip_token(stream) )
        {
            error("[%s][LINE %d][EXPECTED BYTE]", __func__, stream->line);
        }
        
        ctx->byte_counter++;

    } while (stream_expect(stream, SEQUENCE_SEPARATOR) );
}

/**********/
static void byte_b(Context *ctx, Stream *in, Stream *out)
{
    stream_skip_blanks(in);

    do
    {
        char *token = stream_get_token(in);

        if(!token)
        {
            error("[%s][LINE %d][EXPECTED BYTE]", __func__, in->line);
        }
        
        uint16_t val = 0;
        if(!str_to_word(token, &val) || val>0xFF)
        {
            error("[%s][LINE %d][INVALID BYTE VALUE]", __func__, in->line);
        }

        free(token);

        stream_write_byte(out, (uint8_t)val);

        ctx->byte_counter++;

    } while (stream_expect(in, SEQUENCE_SEPARATOR) );
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
    char *start = &stream->buffer[stream->pos];

    stream_skip_until(stream, STRING_DELIMITER);

    char *end = &stream->buffer[stream->pos];

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
    char *start = &in->buffer[in->pos];

    stream_skip_until(in, STRING_DELIMITER);

    char *end = &in->buffer[in->pos];

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

    char *start = &stream->buffer[stream->pos];

    stream_skip_until(stream, STRING_DELIMITER);

    char *end = &stream->buffer[stream->pos];

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

    //parse the file
    assembler_parse(ctx, _stream);

    free(file_name);

    stream_close(_stream);
}

/**********/
static void src_b(Context *ctx, Stream *in, Stream *out)
{
    stream_skip_blanks(in);

    //string start
    stream_expect(in, STRING_DELIMITER);

    char *start = &in->buffer[in->pos];

    stream_skip_until(in, STRING_DELIMITER);

    char *end = &in->buffer[in->pos];

    //expect string end
    stream_expect(in, STRING_DELIMITER);

    //get file name
    char *file_name = alloc( (end-start)+1, sizeof(char));

    memcpy(file_name, start, (end-start));

    //parse the include file
    Stream *_stream = stream_init(file_name);

    assembler_build(ctx, _stream, out);

    free(file_name);

    stream_close(_stream);
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

    char *start = &stream->buffer[stream->pos];

    stream_skip_until(stream, STRING_DELIMITER);

    char *end = &stream->buffer[stream->pos];

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

    char *start = &in->buffer[in->pos];

    stream_skip_until(in, STRING_DELIMITER);

    char *end = &in->buffer[in->pos];

    //expect string end
    stream_expect(in, STRING_DELIMITER);

    //get file name
    char *file_name = alloc( (end-start)+1, sizeof(char));

    memcpy(file_name, start, (end-start));

    //load the minary file
    Stream *_stream = stream_init(file_name);

    //copy it to output
    memcpy(&out->buffer[out->pos], _stream->buffer, _stream->buffer_size);

    //adjust the byte counter AND buffer position
    out->pos +=  _stream->buffer_size;
    ctx->byte_counter += _stream->buffer_size;

    free(file_name);

    stream_close(_stream);
}

/**********/
static Macro MACROS[] = 
{
    {"ORG", org_p, org_b},
    {"BYTE", byte_p, byte_b},
    {"STRING", string_p, string_b},
    {"INCLUDE_SRC", src_p, src_b},
    {"INCLUDE_BIN", bin_p, bin_b}
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
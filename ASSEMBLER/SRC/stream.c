#define _CRT_SECURE_NO_WARNINGS

#include "funcs.h"
#include "stream.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**********/
Stream *stream_init(char *file)
{
    Stream *stream = alloc(1, sizeof(Stream));

    stream->line = 1;
    
    load_buffer(file, &stream->buffer, &stream->buffer_size);
    
    return stream;
}

/**********/
Stream *stream_allocate(size_t size)
{
    Stream *stream = alloc(1, sizeof(Stream));

    stream->buffer = alloc(size, sizeof(uint8_t));

    stream->buffer_size = size;

    return stream;
}

/**********/
void stream_close(Stream *stream)
{
    free(stream->buffer);

    free(stream);
}

/**********/
uint8_t stream_eof(Stream *stream)
{
    return stream->pos >= stream->buffer_size;
}

/**********/
void stream_skip_blanks(Stream *stream)
{
    while(stream->pos<stream->buffer_size && isspace(stream->buffer[stream->pos]) && stream->buffer[stream->pos]!=EOL)
    { 
        stream->pos++; 
    }    
}

/**********/
void stream_skip_until(Stream *stream, char c)
{
    while(stream->pos<stream->buffer_size && stream->buffer[stream->pos]!=c)
    { 
        if(stream->buffer[stream->pos]==EOL) { stream->line++; }//not 100% sure...

        stream->pos++; 
    }    
}

/**********/
uint8_t stream_expect(Stream *stream, char c)
{
    if(stream->pos<stream->buffer_size)
    {
        if(stream->buffer[stream->pos]==c) 
        {
            if(stream->buffer[stream->pos]==EOL) 
            { stream->line++; }

            stream->pos++;

            return 1;
        }
    }

    return 0;
}

/**********/
char *stream_get_token(Stream *stream)
{
    size_t pos = stream->pos;
    uint32_t count = 0;

    while(pos<stream->buffer_size && (isalnum(stream->buffer[pos]) || stream->buffer[pos]==UNDERSCORE) ) 
    { 
        pos++; 
        count++; 
    }  
  
    if(count>0)
    {
        char *res = alloc(count+1, sizeof(char));

        memcpy(res, &stream->buffer[stream->pos], count);

        stream->pos += count;

        return res;
    }

    return NULL;
}

/**********/
//skip a token, 0 if failed
uint8_t stream_skip_token(Stream *stream)
{
    size_t pos = stream->pos;
    size_t count = 0;

    while(pos<stream->buffer_size && (isalnum(stream->buffer[pos]) || stream->buffer[pos]==UNDERSCORE) ) 
    { 
        pos++; 
        count++; 
    }  
  
    stream->pos += count;

    return count != 0;
}

/**********/
void stream_write_byte(Stream *stream, uint8_t byte)
{
    if(stream_eof(stream)){ error("[%s][WRITE EXCEED BUFFER SIZE]"); }

    stream->buffer[stream->pos] = byte;

    stream->pos++;
}

/**********/
void stream_write_word(Stream *stream, uint16_t word)
{            
    stream_write_byte(stream, (uint8_t)(word>>8));

    stream_write_byte(stream, (uint8_t)(word & 0x00FF));
}

/**********/
void stream_to_file(Stream *stream, char *file)
{
    save_buffer(file, stream->buffer, stream->pos);
}



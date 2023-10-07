#ifndef STREAM_H
#define STREAM_H

#include <stdint.h>

#define EOL '\n'
#define UNDERSCORE '_'
#define COMMA ','
#define STRING_DELIMITER '"'

typedef struct Stream
{
    uint8_t *buffer;
    size_t buffer_size;

    size_t pos;
	uint32_t line;

} Stream;

Stream *stream_init(char *file);

Stream *stream_allocate(size_t);

void stream_close(Stream *stream);

uint8_t stream_eof(Stream *stream);

void stream_skip_blanks(Stream *stream);

void stream_skip_until(Stream *stream, char c);

uint8_t stream_expect(Stream *stream, char c);

char *stream_get_token(Stream *stream);

uint8_t stream_skip_token(Stream *stream);

void stream_write_byte(Stream *stream, uint8_t byte);

void stream_write_word(Stream *stream, uint16_t word);

void stream_to_file(Stream *stream, char *file);

#endif
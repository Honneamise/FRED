#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
    #include <crtdbg.h>
#endif

/*#ifndef _CRTDBG_ALLOC_MEM_DF 
    #define _CRTDBG_ALLOC_MEM_DF 1
#endif

#ifndef _CRTDBG_LEAK_CHECK_DF
    #define _CRTDBG_LEAK_CHECK_DF 32
#endif*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdarg.h>

/**********/
void error(const char * format, ...)
{
    #ifdef _DEBUG
        int32_t flag = 0;

        flag &= ~_CRTDBG_ALLOC_MEM_DF;
        _CrtSetDbgFlag(flag);

        flag &= ~_CRTDBG_LEAK_CHECK_DF;
        _CrtSetDbgFlag(flag);
    #endif

    fprintf(stdout,"[*ERROR*]");

    va_list argp = {0};
	va_start(argp, format);
	vfprintf(stdout, format, argp);
	va_end(argp);

    fprintf(stdout,"\n");

    exit(-1);
}

/**********/
void *alloc(size_t count, size_t size)
{
	void *p = calloc(count, size);

	if (p == NULL) { error("[%s][MEMORY ALLOCATION FAILED]",__func__); };

	return p;
}

/**********/
void load_buffer(char *file, uint8_t **buffer, size_t *size)
{
	size_t s = 0;

	FILE *fp = fopen(file, "rb");

	if (!fp) { error("[%s][FAILED TO OPEN FILE][%s]", __func__, file); } 
	else
	{
		if(fseek(fp, 0, SEEK_END) != 0) { error("[%s][FSEEK FAILED ON FILE][%s]", __func__, file); }

		s = ftell(fp);

        if( s == -1L && errno >= 0) { error("[%s][FTELL FAILED ON FILE][%s]", __func__, file); }

		*buffer = alloc(s, sizeof(uint8_t));

		if(fseek(fp, 0, SEEK_SET) != 0) { error("[%s][FSEEK FAILED ON FILE][%s]", __func__, file); }

		if(fread(*buffer, s, 1, fp) != 1 ) { error("[%s][FREAD FAILED ON FILE][%s]", __func__, file); }

		if(fclose(fp) != 0) { error("[%s][FCLOSE FAILED ON FILE][%s]", __func__, file); }
	}

	if(size){ *size = s; }
}

/**********/
void save_buffer(char *file, uint8_t *buffer, uint32_t size)
{
	FILE *fp = fopen(file, "wb");

	if (!fp) { error("[%s][FAILED TO OPEN FILE][%s]", __func__, file); } 
	else
	{
		if(fwrite(buffer, size, 1, fp) != 1) { error("[%s][FWRITE FAILED ON FILE][%s]", __func__, file); }

		if(fclose(fp) != 0) { error("[%s][FCLOSE FAILED ON FILE][%s]", __func__, file); }
	}
}

/**********/
// return 1 conversion success, 0 fail
uint8_t str_to_word(char *str, uint16_t *num)
{
    char *start = str;
    char *end = NULL;

    errno = 0;

    uintmax_t val = strtoumax(start, &end, 0);

    if(start==end || errno!=0) { return 0; }

    if(val > 0xFFFF) { return 0; }

    if(num) { *num = (uint16_t)val; }

    return 1;
}
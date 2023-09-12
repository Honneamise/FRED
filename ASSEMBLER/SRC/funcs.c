#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
    #include <crtdbg.h>
#endif

#ifndef _CRTDBG_ALLOC_MEM_DF 
    #define _CRTDBG_ALLOC_MEM_DF 1
#endif

#ifndef _CRTDBG_LEAK_CHECK_DF
    #define _CRTDBG_LEAK_CHECK_DF 32
#endif

#ifndef va_list
    typedef char* va_list;
#endif
        
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdarg.h>
#include <windows.h>

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
//TODO : add more checks
void load_buffer(char *file, uint8_t **buffer, uint32_t *size)
{
	size_t _size = 0;

	FILE *fp = fopen(file, "rb");

	if (fp == NULL) { error("[%s][FAILED TO OPEN FILE][%s]", __func__, file); } 
	else
	{
		fseek(fp, 0, SEEK_END);

		_size = ftell(fp);

		*buffer = alloc( _size, sizeof(uint8_t));

		fseek(fp, 0, SEEK_SET);

		size_t check = fread(*buffer, _size, 1, fp);

		fclose(fp);
	}

	if(size){ *size = (uint32_t)_size; }
}

/**********/
//TODO: add more checks
void save_buffer(char *file, uint8_t *buffer, uint32_t size)
{
	size_t _size = 0;

	FILE *fp = fopen(file, "wb");

	if (fp == NULL) { error("[%s][FAILED TO OPEN FILE][%s]", __func__, file); } 
	else
	{
		size_t check = fwrite(buffer, size, 1, fp);

		fclose(fp);
	}

}

/**********/
void usleep(int64_t usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * (int64_t)usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);

    if(!timer){ error("[%s][INTERNAL TIMER CREATION FAILED]", __func__); return; }
	
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	
    WaitForSingleObject(timer, INFINITE);
	
    CloseHandle(timer);
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
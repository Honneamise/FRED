#ifdef _DEBUG
    #include <crtdbg.h>
#endif

#ifndef _CRTDBG_ALLOC_MEM_DF 
    #define _CRTDBG_ALLOC_MEM_DF 1
#endif

#ifndef _CRTDBG_LEAK_CHECK_DF
    #define _CRTDBG_LEAK_CHECK_DF 32
#endif

#define DEFAULT_CPU_MHZ         1
#define DEFAULT_CHARSET_FILE    "charset.rom"
#define DEFAULT_PROGRAM_FILE    "program.bin"

#include "funcs.h"
#include "system.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    #ifdef _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    #endif

    if(argc == 1) //default 
    { 
        system_init(DEFAULT_CHARSET_FILE, DEFAULT_PROGRAM_FILE); 

        system_run(DEFAULT_CPU_MHZ);
    }
    else if(argc == 4) //get user params 
    { 
        uint16_t speed = 0;

        if(!str_to_word(argv[1], &speed) || speed <1 || speed>0xFF)
        {
            error("[CPU SPEED INVALID][MIN-MAX 1-255]"); 
        }

        system_init(argv[2], argv[3]); 

        system_run((uint8_t)speed);
    }
    else //error, print usage
    { 
        error("[USAGE][FRED_EMU.EXE CPU_MHZ CHARSET_FILE PROGRAM_FILE]"); 
    }

    system_close();
    
    return 0;
}
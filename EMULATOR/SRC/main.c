#ifdef _DEBUG
    #include <crtdbg.h>
#endif

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

    if(argc !=4 )
    { 
        error("[USAGE][FRED_EMU.EXE CPU_MHZ CHARSET_FILE PROGRAM_FILE]"); 
    }
    
    uint16_t speed = 0;

    if(!str_to_word(argv[1], &speed) || speed <1 || speed>0xFF)
    {
        error("[CPU SPEED INVALID][MIN-MAX 1-255]"); 
    }

    system_init(argv[2], argv[3]); 

    system_run((uint8_t)speed);
  
    system_close();
    
    return 0;
}
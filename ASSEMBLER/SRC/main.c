#ifdef _DEBUG
    #include <crtdbg.h>
#endif

#include "funcs.h"
#include "macro.h"
#include "parser.h"
#include "builder.h"

#include <stdio.h>

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
    
    if(argc != 3)
    {
        error(__func__,"USAGE : FRED_ASS.EXE INPUT_FILE OUTPUT_FILE"); 
    }

    Context *ctx = context_init();

    Stream *in = stream_init(argv[1]);

    Stream *out = stream_allocate(0xEFFF);

    printf("PARSING...");
    assembler_parse(ctx, in);
    printf("OK\n");

    //reset the input stream
    in->pos = 0;
    in->line = 1;

    printf("BUILDING...");
    assembler_build(ctx, in, out);
    printf("OK\n");

    stream_to_file(out, argv[2]);

    context_close(ctx);

    stream_close(in);

    stream_close(out);

    return 0;
}
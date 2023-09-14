#include "funcs.h"
#include "context.h"
#include "stream.h"
#include "isa.h"
#include "macro.h"

#include <stdlib.h>
#include <string.h>

/**********/
//parsing errors should already be addressed
//just check for params when building instructions
void assembler_build(Context *ctx, Stream *in, Stream *out)
{
    char *token = NULL;

    while(!stream_eof(in))
    {
        stream_skip_blanks(in);

        token = stream_get_token(in);

        //token can be symbol, instruction or macro
        if(token != NULL) 
        { 
            //if symbol skip it
            if(stream_expect(in, SYMBOL_MARKER))
            {
                free(token);

                //we consumed the token, get next token if any
                stream_skip_blanks(in);
                token = stream_get_token(in);
            }

            //instruction or macro
            if(token != NULL)
            {
                void *ptr = NULL;

                if( ptr = isa_get_instruction(token) )//instruction ?
                {
                    isa_build(ctx, in, out, ptr);
                }

                else if ( ptr = macro_get(token) )//macro ?
                {
                    macro_build(ctx, in, out, ptr);
                }

                free(token);
            }    
        }
        
        stream_skip_blanks(in);

        //comment present ?
        if(stream_expect(in, COMMENT_START))
        {
            stream_skip_until(in, EOL);
        }

        //if not EOF check for EOL and proceed to next line
        if(!stream_eof(in)) { stream_expect(in, EOL); }
        
    }
    
}

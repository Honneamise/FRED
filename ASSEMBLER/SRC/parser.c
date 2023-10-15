#include "funcs.h"
#include "context.h"
#include "stream.h"
#include "isa.h"
#include "macro.h"
#include "parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**********/
static void parse_symbol(Context *ctx, Stream *stream, char *token)
{
    if( //isa_get_instruction(token) ||  //no instr    
        macro_get(token) ||            //no macro
        str_to_word(token, NULL) ||    //no number
        isa_get_register(token)!=-1 )  //no register
    { 
        error("[%s][LINE %d][INVALID SYMBOL NAME][%s]", __func__, stream->line, token);
    }

    //is symbol duplicated ?
    if(context_get_symbol(ctx, token)) 
    { 
        error("[%s][LINE %d][DUPLICATED SYMBOL FOUND][%s]", __func__, stream->line, token);
    }                

    //OK, valid !!!
    context_add_symbol(ctx, token);
}

/**********/
void assembler_parse(Context *ctx, Stream *stream)
{
    char *token = NULL;

    while(!stream_eof(stream))
    {
        stream_skip_blanks(stream);

        token = stream_get_token(stream);

        //token can be symbol, instruction or macro
        if(token != NULL) 
        { 
            //symbol definition ?
            if(stream_expect(stream, COLON))
            {
                parse_symbol(ctx, stream, token);

                //we consumed the token, get next token if any
                stream_skip_blanks(stream);
                token = stream_get_token(stream);
            }

            //instruction or macro
            if(token != NULL)
            {
                void *ptr = NULL;

                if( (ptr = isa_get_instruction(token)) != NULL )//instruction ?
                {
                    isa_parse(ctx, stream, ptr);
                }

                else if ( (ptr = macro_get(token)) != NULL )//macro ?
                {
                    macro_parse(ctx, stream, ptr);
                }

                else //error
                {
                    error("[%s][LINE %d][INVALID TOKEN][%s]", __func__, stream->line, token);
                }

                free(token);
            }    
        }
        
        stream_skip_blanks(stream);

        //comment present with ";"
        if(stream_expect(stream, SEMICOLON))
        {
            stream_skip_until(stream, EOL);
        }

        //comment present with ".."
        if(stream_expect(stream, DOT) && stream_expect(stream, DOT))
        {
            stream_skip_until(stream, EOL);
        }

        //if not EOF check for EOL and proceed to next line
        if(!stream_eof(stream) && !stream_expect(stream, EOL))
        {
            error("[%s][LINE %d][INVALID LINE]", __func__, stream->line);
        }
    }

}
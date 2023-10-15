#include "context.h"
#include "funcs.h"
#include <stdlib.h>
#include <string.h>

/**********/
Context *context_init(void)
{
    return alloc(1, sizeof(Context));
}

/**********/
void context_close(Context *ctx)
{
    Symbol *s = ctx->table;

    while(s)
    {
        ctx->table = s->next;

        free(s->name);
        
        free(s);

        s = ctx->table;
    }

    free(ctx);
}

/**********/
void context_add_symbol(Context *ctx, char *name)
{
    Symbol *s = alloc(1, sizeof(Symbol));

    s->name = name;
    
    if(ctx->byte_counter>0xFFFF)
    {
        error("[%s][LABEL REFERENCE INVALID][%s][%X]", __func__, s->name, ctx->byte_counter);            
    }

    s->val = (uint16_t)ctx->byte_counter;
   
    s->next = ctx->table;

    ctx->table = s;
}

/**********/
Symbol *context_get_symbol(Context *ctx, char *name)
{
    Symbol *s = ctx->table;

    while(s)
    {
       if( strcmp(name, s->name)==0 ) { return s; }

        s = s->next;
    }

    return NULL;
}

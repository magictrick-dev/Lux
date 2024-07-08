#ifndef SOURCE_COMPILER_LEXER_H
#define SOURCE_COMPILER_LEXER_H
#include <core/definitions.h>
#include <compiler/token.h>

typedef struct tokenizer 
{
    const char *source;
    u64 step;
    u64 offset;
} source_lexicon;

b32     tokenizer_is_eol(tokenizer *state);
b32     tokenizer_is_eof(tokenizer *state);
b32     get_next_token(tokenizer *state, source_token *token);

#endif

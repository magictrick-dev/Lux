#ifndef SOURCE_COMPILER_LEXER_H
#define SOURCE_COMPILER_LEXER_H
#include <core/definitions.h>
#include <compiler/token.h>

typedef struct source_lexicon
{
    const char *source;
    u64 step;
    u64 offset;
} source_lexicon;

b32 get_next_token(source_lexicon *lexicon, source_token *token);

#endif

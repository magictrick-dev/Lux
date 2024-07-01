#ifndef SOURCE_COMPILER_TOKEN_H
#define SOURCE_COMPILER_TOKEN_H
#include <core/definitions.h>

typedef enum token_type : u32
{
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVISION,
    ASSIGNMENT,
    IMPLIED_ASSIGNMENT,
    LESS_THAN,
    LESS_THAN_EQUALS,
    GREATER_THAN,
    GREATER_THAN_EQUALS,
    EQUALS,
    NOT_EQUALS,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    AND,
    OR,
    NOT,
    XOR,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COLON,
    COLON_CONSTANT,
    SEMICOLON,
    HASH_DIRECTIVE
} token_type;

typedef struct source_token
{
    u32 type;

    struct
    {
        const char *source_buffer;
        u64         source_offset;
    } Lexeme;

} source_token;

#endif

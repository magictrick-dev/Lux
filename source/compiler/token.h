#ifndef SOURCE_COMPILER_TOKEN_H
#define SOURCE_COMPILER_TOKEN_H
#include <core/definitions.h>

typedef enum token_type : u32
{
    PLUS,
    PLUS_EQUALS,
    PLUS_PLUS,
    MINUS,
    MINUS_EQUALS,
    MINUS_MINUS,
    STAR,
    STAR_EQUALS,
    FORWARD_SLASH,
    FORWARD_SLASH_EQUALS,
    LESS_THAN,
    LESS_THAN_EQUALS,
    LESS_THAN_LESS_THAN,
    GREATER_THAN,
    GREATER_THAN_EQUALS,
    GREATER_THAN_GREATER_THAN,
    EQUALS,
    EQUALS_EQUALS,
    BANG,
    BANG_EQUALS,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_ANGLE_BRACKET,
    RIGHT_ANGLE_BRACKET,
    LEFT_CURLY_BRACKET,
    RIGHT_CURLY_BRACKET,
    COLON,
    COLON_EQUALS,
    SEMICOLON,
    AMPERSAND,
    AMPERSAND_AMPERSAND,
    PIPE,
    PIPE_PIPE,

    INTEGER,
    REAL,
    HEXADECIMAL,
    IDENTIFIER,

    UNDEFINED,
} token_type;

typedef struct source_token
{

    u32 type;

    struct
    {
        const char *source;
        u64         offset;
        u64         length;
    } lexeme;

} source_token;

#endif

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
    SINGLE_QUOTE_STRING,
    DOUBLE_QUOTE_STRING,
    IDENTIFIER,
    UNDEFINED,
    UNDEFINED_STR_EOL,
    UNDEFINED_STR_EOF,
    END_OF_FILE,
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

inline const char *
token_type_to_string(u32 type)
{

    switch (type)
    {

        case token_type::PLUS: return "plus";
        case token_type::PLUS_EQUALS: return "plus_equals";
        case token_type::PLUS_PLUS: return "plus_plus";
        case token_type::MINUS: return "minus";
        case token_type::MINUS_EQUALS: return "minus_equals";
        case token_type::MINUS_MINUS: return "minus_minus";
        case token_type::STAR: return "star";
        case token_type::STAR_EQUALS: return "star_equals";
        case token_type::FORWARD_SLASH: return "forward_slash";
        case token_type::FORWARD_SLASH_EQUALS: return "forward_slash_equals";
        case token_type::LESS_THAN: return "less_than";
        case token_type::LESS_THAN_EQUALS: return "less_than_equals";
        case token_type::LESS_THAN_LESS_THAN: return "less_than_less_than";
        case token_type::GREATER_THAN: return "greater_than";
        case token_type::GREATER_THAN_EQUALS: return "greater_than_equals";
        case token_type::GREATER_THAN_GREATER_THAN: return "greater_than_greater_than";
        case token_type::EQUALS: return "equals";
        case token_type::EQUALS_EQUALS: return "equals_equals";
        case token_type::BANG: return "bang";
        case token_type::BANG_EQUALS: return "bang_equals";
        case token_type::LEFT_PARENTHESIS: return "left_parenthesis";
        case token_type::RIGHT_PARENTHESIS: return "right_parenthesis";
        case token_type::LEFT_ANGLE_BRACKET: return "left_angle_bracket";
        case token_type::RIGHT_ANGLE_BRACKET: return "right_angle_bracket";
        case token_type::LEFT_CURLY_BRACKET: return "left_curly_bracket";
        case token_type::RIGHT_CURLY_BRACKET: return "right_curly_bracket";
        case token_type::COLON: return "colon";
        case token_type::COLON_EQUALS: return "colon_equals";
        case token_type::SEMICOLON: return "semicolon";
        case token_type::AMPERSAND: return "ampersand";
        case token_type::AMPERSAND_AMPERSAND: return "ampersand_ampersand";
        case token_type::PIPE: return "pipe";
        case token_type::PIPE_PIPE: return "pipe_pipe";
        case token_type::INTEGER: return "integer";
        case token_type::REAL: return "real";
        case token_type::HEXADECIMAL: return "hexadecimal";
        case token_type::SINGLE_QUOTE_STRING: return "single_quote_string";
        case token_type::DOUBLE_QUOTE_STRING: return "double_quote_string";
        case token_type::IDENTIFIER: return "identifier";
        case token_type::UNDEFINED: return "undefined";
        case token_type::UNDEFINED_STR_EOL: return "undefined_str_eol";
        case token_type::UNDEFINED_STR_EOF: return "undefined_str_eof";
        case token_type::END_OF_FILE: return "end_of_file";
        default: lux_noreach("Unimplemented token type to string case.");

    };

    return "";

}

#endif

#ifndef SOURCE_COMPILER_PARSER_H
#define SOURCE_COMPILER_PARSER_H
#include <compiler/token.h>
#include <compiler/lexer.h>
#include <core/definitions.h>
#include <core/allocator.h>

typedef struct parser
{
    memory_arena   *arena;
    tokenizer       tokenizer_state;

    source_token    tokens[3];
    source_token   *peek_token      = NULL;
    source_token   *current_token   = NULL;
    source_token   *previous_token  = NULL;

} parser;

typedef enum operation_type : u32
{
    OP_ADDITION,
    OP_SUBTRACTION,
    OP_MULTIPLICATION,
    OP_DIVISION,
    OP_EQUALS_TO,
    OP_NOT_EQUALS_TO,
    OP_LESS_THAN,
    OP_LESS_THAN_EQUALS_TO,
    OP_GREATER_THAN,
    OP_GREATER_THAN_EQUALS_TO,
    OP_SIGN_NEGATIVE,
    OP_NEGATION,
    OP_INVALID,
} operation_type;

typedef enum literal_type : u32
{
    LITERAL_SIGNED_INTEGER,
    LITERAL_UNSIGNED_INTEGER,
    LITERAL_REAL,
    LITERAL_STRING,
    LITERAL_BOOLEAN,
} literal_type;

typedef struct literal_object
{

    literal_type    type;

    union
    {
        const char* string;
        i64         signed_integer;
        u64         unsigned_integer;
        double      real;
    };

} literal_object;

struct binary_expression_node
{
    void   *left;
    void   *right;
    u32     operation;
};

struct unary_expression_node
{
    void   *center;
    u32     operation;
};

struct primary_expression_node
{
    literal_object object;
};

struct grouping_expression_node
{
    void    *center;
};

typedef enum syntax_node_type : u32
{
    BINARY_EXPRESSION,
    UNARY_EXPRESSION,
    PRIMARY_EXPRESSION,
    GROUPING_EXPRESSION,
} syntax_node_type;

struct syntax_node
{

    u32 type;
    
    union
    {
        binary_expression_node      binary_expression;
        unary_expression_node       unary_expression;
        primary_expression_node     primary_expression;
        grouping_expression_node    grouping_expression;
    };

};

// --- Parser Grammer ----------------------------------------------------------
//
// Expressions:
//      expression      : equality
//      equality        : comparison (( "!=" | "==" ) comparison)*
//      comparison      : term (( "<" | ">" | "<=" | ">=") term)*
//      term            : factor (( "+" | "-") factor)*
//      factor          : unary (( "*" | "/") unary)*
//      unary           : ( "!" | "-" ) unary | primary
//      primary         : NUMBER | STRING | "true" | "false" | "(" expression ")"
//

b32             initialize_parser(parser *state, memory_arena *arena, const char *source_buffer);
syntax_node*    recursively_descend_primary(parser *state);
syntax_node*    recursively_descend_unary(parser *state);
syntax_node*    recursively_descend_factor(parser *state);
syntax_node*    recursively_descend_term(parser *state);
syntax_node*    recursively_descend_comparison(parser *state);
syntax_node*    recursively_descend_equality(parser *state);
syntax_node*    recursively_descend_expression(parser *state);

// --- Parse Traversal ---------------------------------------------------------
//
// Primarily for debugging the output.
//

void output_ast(syntax_node *node);

#endif

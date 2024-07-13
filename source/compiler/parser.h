#ifndef SOURCE_COMPILER_PARSER_H
#define SOURCE_COMPILER_PARSER_H
#include <compiler/token.h>
#include <core/definitions.h>

struct binary_expression_node;
struct unary_expression_node;
struct primary_expression_node;
struct grouping_expression_node;
struct syntax_node;

typedef struct parser
{
    tokenizer tokenizer_state;
    source_token current_token;
    source_token previous_token;
} parser;

typedef enum syntax_node_type : u32
{
    BINARY_EXPRESSION,
    UNARY_EXPRESSION,
    PRIMARY_EXPRESSION,
    GROUPING_EXPRESSION,
} syntax_node_type;

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
} operation_type;

typedef enum literal_type : u32
{
    LITERAL_INTEGER,
    LITERAL_REAL,
    LITERAL_STRING,
} literal_type;

typedef struct literal_object
{

    literal_type    type;

    union
    {
        const char* string;
        u64         integer;
        double      real;
    };

} literal_object;

typedef struct binary_expression_node
{
    syntax_node    *left;
    syntax_node    *right;
    operation_type  operation;
} binary_expression_node;

typedef struct unary_expression_node
{
    syntax_node    *center;
    operation_type  operation;
} unary_expression_node;

typedef struct primary_expression_node
{
    literal_object object;
} primary_expression_node;

typedef struct grouping_expression_node
{
    syntax_node    *center;
} grouping_expression_node;

typedef struct syntax_node
{

    u32 type;
    
    union
    {
        binary_expression_node      binary_expression;
        unary_expression_node       unary_expression;
        primary_expression_node     primary_expression;
        grouping_expression_node    grouping_expression;
    };

} syntax_node;

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

syntax_node* recursively_descend_expression(parser *state);

#endif

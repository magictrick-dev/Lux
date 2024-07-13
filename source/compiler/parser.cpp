#include <compiler/parser.h>

// --- Parser Helpers ----------------------------------------------------------

typedef struct token_type_match_list
{
    u32 types[8];
    u64 count;
} token_type_match_list;

static inline token_type_match_list
match_list(u32 count, ...)
{
    
    lux_assert(count <= 8);

    token_type_match_list list = {};
    list.count = count;

    va_list types_list;
    va_start(types_list, count);

    for (i32 index = 0; index < count; ++index)
        list.types[index] = va_arg(types_list, u32);

    va_end(types_list);

    return list;

}

static inline u32
token_type_to_operation_type(u32 type)
{
    
    switch (type)
    {
        case token_type::EQUALS_EQUALS:         { return operation_type::OP_EQUALS_TO; }
        case token_type::BANG_EQUALS:           { return operation_type::OP_NOT_EQUALS_TO; }
        case token_type::LESS_THAN:             { return operation_type::OP_LESS_THAN; }
        case token_type::LESS_THAN_EQUALS:      { return operation_type::OP_LESS_THAN_EQUALS_TO; }
        case token_type::GREATER_THAN:          { return operation_type::OP_GREATER_THAN; }
        case token_type::GREATER_THAN_EQUALS:   { return operation_type::OP_GREATER_THAN_EQUALS_TO; }
        case token_type::PLUS:                  { return operation_type::OP_ADDITION; }
        case token_type::MINUS:                 { return operation_type::OP_SUBTRACTION; }
        case token_type::STAR:                  { return operation_type::OP_MULTIPLICATION; }
        case token_type::FORWARD_SLASH:         { return operation_type::OP_DIVISION; }
        default:                                { lux_noreach("Unimplemented operation."); }
    }

    return operation_type::OP_INVALID;

}

static inline b32
check_token(parser *state, u32 type)
{
    b32 result = (state->current_token->type == type);
    return result;
}

static inline void
consume_token(parser *state)
{
    
    // Shift the tokens.
    source_token *temporary     = state->previous_token;
    state->previous_token       = state->current_token;
    state->current_token        = state->peek_token;
    state->peek_token           = temporary;
    
    // Get the next peek token.
    get_next_token(&state->tokenizer_state, state->peek_token);

}

static inline b32
current_token_matches_from_list(parser *state, token_type_match_list *list)
{

    for (i32 idx = 0; idx < list->count; ++idx)
    {
        
        if (check_token(state, list->types[idx]))
        {
            consume_token(state);
            return true;
        }

    }

    return false;
}

static inline void
convert_token_to_literal(memory_arena *arena, source_token *token, syntax_node *primary_node)
{

    // Save arena state.
    u64 arena_state = arena_save(arena);

    // Copy the string to an internal buffer.
    char *token_string_buffer = arena_push_array(arena, char, token->lexeme.length + 1);
    memcpy(token_string_buffer, token->lexeme.source + token->lexeme.offset, token->lexeme.length);
    token_string_buffer[token->lexeme.length] = '\0';

    switch(token->type)
    {
    
        case token_type::INTEGER:
        {
            i64 result = atoll(token_string_buffer);
            primary_node->primary_expression.object.type = literal_type::LITERAL_SIGNED_INTEGER;
            primary_node->primary_expression.object.signed_integer = result;
        } break;

        case token_type::REAL:
        {
            f64 result = atof(token_string_buffer);
            primary_node->primary_expression.object.type = literal_type::LITERAL_REAL;
            primary_node->primary_expression.object.real = result;
        } break;

        case token_type::HEXADECIMAL:
        {

            // Looks like those NIU dailies are finally paying off.
            u64 accumulator = 0;
            u64 exponent    = 0;
            for (i32 index = token->lexeme.length - 1; index > 1; --index)
            {

                char c = toupper(token_string_buffer[index]);
                u64 result = 0;

                if (c >= 48 && c <= 57) result = (c - 48) * pow(16, exponent);
                else result = (10 + (c - 65)) * pow(16, exponent);

                accumulator += result;
                exponent    += 1;

            }

            primary_node->primary_expression.object.type = literal_type::LITERAL_UNSIGNED_INTEGER;
            primary_node->primary_expression.object.unsigned_integer = accumulator;

        } break;

        default:
        {
            lux_noreach("Literal case is probably invalid or uncaught.");
        } break;
    };

    // Restore arena state.
    arena_restore(arena, arena_state);

}


// --- Parser ------------------------------------------------------------------

b32
initialize_parser(parser *state, memory_arena *arena, const char *source_buffer)
{

    // Initialize the state.
    state->tokenizer_state.source   = source_buffer;
    state->tokenizer_state.step     = 0;
    state->tokenizer_state.offset   = 0;
    state->arena                    = arena;
    state->peek_token               = &state->tokens[0];
    state->current_token            = &state->tokens[1];
    state->previous_token           = &state->tokens[2];

    // Prime the parser with tokens.
    get_next_token(&state->tokenizer_state, state->current_token);
    get_next_token(&state->tokenizer_state, state->peek_token);

    return true;

}

// --- Expression Grammar ------------------------------------------------------

syntax_node*
recursively_descend_primary(parser *state)
{

    token_type_match_list primitive_matches = match_list(3,
            token_type::INTEGER,
            token_type::REAL,
            token_type::HEXADECIMAL);

    // Primitive expressions.
    if (current_token_matches_from_list(state, &primitive_matches))
    {
        
        syntax_node *primary_node = arena_push_type(state->arena, syntax_node);
        primary_node->type = syntax_node_type::PRIMARY_EXPRESSION;
        convert_token_to_literal(state->arena, state->previous_token, primary_node);
        return primary_node;

    }

    // Grouping expressions.
    token_type_match_list bracket_match = match_list(1,
            token_type::LEFT_PARENTHESIS);

    if (current_token_matches_from_list(state, &bracket_match))
    {
        
        syntax_node *expression_node = recursively_descend_expression(state);
        
        if (state->current_token->type == token_type::RIGHT_PARENTHESIS)
        {
            consume_token(state);
            
            syntax_node *grouping_node = arena_push_type(state->arena, syntax_node);
            grouping_node->type = syntax_node_type::GROUPING_EXPRESSION;
            grouping_node->grouping_expression.center = expression_node;
            return grouping_node;

        }

        else
        {
        
            printf("ERROR: Unmatched parenthesis in expression.\n");
            return NULL;

        }

    }

    printf("ERROR: Unexpected token in expression.\n");
    return NULL;

}

syntax_node*
recursively_descend_unary(parser *state)
{

    token_type_match_list matches = match_list(2,
            token_type::BANG,
            token_type::MINUS);

    if (current_token_matches_from_list(state, &matches))
    {
        
        // Manually determine type since minus is an ambiguous operation from
        // signing reals/integers as negative.
        u32 operation = operation_type::OP_NEGATION;
        if (state->previous_token->type == token_type::MINUS)
            operation = operation_type::OP_SIGN_NEGATIVE;

        syntax_node *right = recursively_descend_unary(state);
        syntax_node *unary_node = arena_push_type(state->arena, syntax_node);

        unary_node->type = syntax_node_type::UNARY_EXPRESSION;
        unary_node->unary_expression.center = right;
        unary_node->unary_expression.operation = operation;

        return unary_node;

    }

    syntax_node *primary = recursively_descend_primary(state);
    return primary;

}

syntax_node*
recursively_descend_factor(parser *state)
{

    syntax_node *node = recursively_descend_unary(state);

    token_type_match_list matches = match_list(2, 
            token_type::STAR, 
            token_type::FORWARD_SLASH);

    while (current_token_matches_from_list(state, &matches))
    {

        u32 operation = token_type_to_operation_type(state->previous_token->type);
        syntax_node *right = recursively_descend_unary(state);
        syntax_node *binary_node = arena_push_type(state->arena, syntax_node);

        binary_node->type = syntax_node_type::BINARY_EXPRESSION;
        binary_node->binary_expression.left         = node;
        binary_node->binary_expression.right        = right;
        binary_node->binary_expression.operation    = operation;

        node = binary_node;

    }

    return node;

}

syntax_node*
recursively_descend_term(parser *state)
{

    syntax_node *node = recursively_descend_factor(state);

    token_type_match_list matches = match_list(2, 
            token_type::PLUS, 
            token_type::MINUS);

    while (current_token_matches_from_list(state, &matches))
    {

        u32 operation = token_type_to_operation_type(state->previous_token->type);
        syntax_node *right = recursively_descend_factor(state);
        syntax_node *binary_node = arena_push_type(state->arena, syntax_node);

        binary_node->type = syntax_node_type::BINARY_EXPRESSION;
        binary_node->binary_expression.left         = node;
        binary_node->binary_expression.right        = right;
        binary_node->binary_expression.operation    = operation;

        node = binary_node;

    }

    return node;

}

syntax_node*
recursively_descend_comparison(parser *state)
{

    syntax_node *node = recursively_descend_term(state);

    token_type_match_list matches = match_list(4, 
            token_type::LESS_THAN, 
            token_type::LESS_THAN_EQUALS, 
            token_type::GREATER_THAN, 
            token_type::GREATER_THAN_EQUALS);

    while (current_token_matches_from_list(state, &matches))
    {

        u32 operation = token_type_to_operation_type(state->previous_token->type);
        syntax_node *right = recursively_descend_term(state);
        syntax_node *binary_node = arena_push_type(state->arena, syntax_node);

        binary_node->type = syntax_node_type::BINARY_EXPRESSION;
        binary_node->binary_expression.left         = node;
        binary_node->binary_expression.right        = right;
        binary_node->binary_expression.operation    = operation;

        node = binary_node;

    }

    return node;

}

syntax_node*
recursively_descend_equality(parser *state)
{

    syntax_node *node = recursively_descend_comparison(state);

    token_type_match_list matches = match_list(2, 
            token_type::BANG_EQUALS, 
            token_type::EQUALS_EQUALS);

    while (current_token_matches_from_list(state, &matches))
    {

        u32 operation = token_type_to_operation_type(state->previous_token->type);
        syntax_node *right = recursively_descend_comparison(state);
        syntax_node *binary_node = arena_push_type(state->arena, syntax_node);

        binary_node->type = syntax_node_type::BINARY_EXPRESSION;
        binary_node->binary_expression.left         = node;
        binary_node->binary_expression.right        = right;
        binary_node->binary_expression.operation    = operation;

        node = binary_node;

    }

    return node;

}

syntax_node* 
recursively_descend_expression(parser *state)
{

    // Generate the tree.
    syntax_node *node = recursively_descend_equality(state);
    return node;

}

// --- Parse Traversal ---------------------------------------------------------

void 
output_ast(syntax_node *node)
{

    switch (node->type)
    {

        case syntax_node_type::BINARY_EXPRESSION:
        {

            output_ast((syntax_node*)node->binary_expression.left);
            printf(" ");

            switch (node->binary_expression.operation)
            {
                case operation_type::OP_ADDITION:               { printf("+");  } break;
                case operation_type::OP_SUBTRACTION:            { printf("-");  } break;
                case operation_type::OP_MULTIPLICATION:         { printf("*");  } break;
                case operation_type::OP_DIVISION:               { printf("/");  } break;
                case operation_type::OP_EQUALS_TO:              { printf("=="); } break;
                case operation_type::OP_NOT_EQUALS_TO:          { printf("!="); } break;
                case operation_type::OP_LESS_THAN:              { printf("<");  } break;
                case operation_type::OP_LESS_THAN_EQUALS_TO:    { printf("<="); } break;
                case operation_type::OP_GREATER_THAN:           { printf(">");  } break;
                case operation_type::OP_GREATER_THAN_EQUALS_TO: { printf(">="); } break;
                default: lux_noreach("Unaccounted for binary print.");
            }

            printf(" ");
            output_ast((syntax_node*)node->binary_expression.right);

        } break;

        case syntax_node_type::UNARY_EXPRESSION:
        {

            switch (node->unary_expression.operation)
            {
                case operation_type::OP_SIGN_NEGATIVE: printf("-");
                case operation_type::OP_NEGATION: printf("!");
                default: lux_noreach("Unaccounted for unary print.");
            }

            output_ast((syntax_node*)node->unary_expression.center);

        } break;

        case syntax_node_type::GROUPING_EXPRESSION:
        {

            printf("( ");
            output_ast((syntax_node*)node->grouping_expression.center);
            printf(" )");

        } break;

        case syntax_node_type::PRIMARY_EXPRESSION:
        {

            switch (node->primary_expression.object.type)
            {

                case literal_type::LITERAL_SIGNED_INTEGER:
                {
                    printf("%lli", node->primary_expression.object.signed_integer);
                } break;

                case literal_type::LITERAL_UNSIGNED_INTEGER:
                {
                    printf("%lld", node->primary_expression.object.unsigned_integer);
                } break;

                case literal_type::LITERAL_REAL:
                {
                    printf("%f", node->primary_expression.object.real);
                } break;

                default: lux_noreach("Unaccounted for primary print.");

            };

        } break;

        default:
        {
            lux_noreach("Unaccounted for ast type.");
            return;
        } break;
    };

}


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
    get_next_token(state, state->current_token);
    get_next_token(state, state->peek_token);

    return true;

}

// --- Expression Grammar ------------------------------------------------------

syntax_node*
recursively_descend_comparison(parser *state)
{
    return NULL;
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


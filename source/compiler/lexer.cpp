#include <compiler/lexer.h>

b32
tokenizer_is_eol(tokenizer *state)
{
    b32 result = (state->source[state->step] == '\n');
    return result;
}

b32     
tokenizer_is_eof(tokenizer *state)
{
    b32 result = (state->source[state->step] == '\0');
    return result;
}

static inline char
peek_symbol(tokenizer *state)
{
    char current = state->source[state->step];
    return current;
}

static inline char
peek_symbol_at(tokenizer *state, i32 offset)
{
    char at = state->source[state->step + offset];
    return at;
}

static inline char
consume_symbol(tokenizer *state)
{
    char current = state->source[state->step++];
    return current;
}

static inline void
initialize_token(tokenizer *state, source_token *token, u32 type)
{
    token->type = type;
    token->lexeme.source = state->source;
    token->lexeme.offset = state->offset;
    token->lexeme.length = state->step - state->offset;

}

static inline b32
match_symbols_and_tokenize(tokenizer *state, source_token *token)
{

    char c = peek_symbol(state);
    switch (c)
    {

        case '+':
        {
            char peek = peek_symbol_at(state, 1);
            if (peek == '=')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::PLUS_EQUALS);
                state->offset = state->step;
                return true;
            }
            else if (peek == '+')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::PLUS_PLUS);
                state->offset = state->step;
                return true;
            }
            else
            {
                consume_symbol(state);
                initialize_token(state, token, token_type::PLUS);
                state->offset = state->step;
                return true;
            }
        } break;

        case '-':
        {
            char peek = peek_symbol_at(state, 1);
            if (peek == '=')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::MINUS_EQUALS);
                state->offset = state->step;
                return true;
            }
            else if (peek == '-')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::MINUS_MINUS);
                state->offset = state->step;
                return true;
            }
            else
            {
                consume_symbol(state);
                initialize_token(state, token, token_type::MINUS);
                state->offset = state->step;
                return true;
            }
        } break;

        case '*':
        {
            char peek = peek_symbol_at(state, 1);
            if (peek == '=')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::STAR_EQUALS);
                state->offset = state->step;
                return true;
            }
            else
            {
                consume_symbol(state);
                initialize_token(state, token, token_type::STAR);
                state->offset = state->step;
                return true;
            }
        } break;
        
        case '/':
        {

            char peek = peek_symbol_at(state, 1);
        
            if (peek == '=')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::FORWARD_SLASH_EQUALS);
                state->offset = state->step;
                return true;
            }

            else
            {
                consume_symbol(state);
                initialize_token(state, token, token_type::FORWARD_SLASH);
                state->offset = state->step;
                return true;
            }

        } break;

        case '<':
        {

            char peek = peek_symbol_at(state, 1);
            if (peek == '=')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::LESS_THAN_EQUALS);
                state->offset = state->step;
                return true;
            }
            
            else if (peek == '<')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::LESS_THAN_LESS_THAN);
                state->offset = state->step;
                return true;
            }

            else
            {
                consume_symbol(state);
                initialize_token(state, token, token_type::LESS_THAN);
                state->offset = state->step;
                return true;
            }

        } break;

        case '>':
        {

            char peek = peek_symbol_at(state, 1);
            if (peek == '=')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::GREATER_THAN_EQUALS);
                state->offset = state->step;
                return true;
            }
            
            else if (peek == '>')
            {
                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::GREATER_THAN_GREATER_THAN);
                state->offset = state->step;
                return true;
            }

            else
            {
                consume_symbol(state);
                initialize_token(state, token, token_type::GREATER_THAN);
                state->offset = state->step;
                return true;
            }

        } break;

    }

    return false;

}

static inline b32
match_whitespace(tokenizer *state, source_token *token)
{

    char c = peek_symbol(state);
    switch (c)
    {
        case ' ':   consume_symbol(state); return true;
        case '\r':  consume_symbol(state); return true;
        case '\n':  consume_symbol(state); return true;
        case '\t':  consume_symbol(state); return true;
    }
    
    return false;

}

static inline b32
match_comments(tokenizer *state, source_token *token)
{
    
    if (peek_symbol(state) != '/') return false;
    char peek_next = peek_symbol_at(state, 1);

    switch (peek_next)
    {
    
        case '/':
        {
            consume_symbol(state);
            consume_symbol(state);
            while (!tokenizer_is_eof(state) && !tokenizer_is_eol(state)) consume_symbol(state);
            state->offset = state->step;
            return true;
        } break;

    };

    return false;
}

b32 
get_next_token(tokenizer *state, source_token *token)
{

    // Clear out non-renderable symbols.
    while (match_whitespace(state, token));
    while (match_comments(state, token));
    while (match_whitespace(state, token));
    if (tokenizer_is_eof(state)) return false;
    state->offset = state->step;
    
    // Match symbols.
    if (match_symbols_and_tokenize(state, token)) return true;

    // Undefined symbol encountered.
    consume_symbol(state);
    initialize_token(state, token, token_type::UNDEFINED);
    state->offset = state->step;
    return true;

    
    return false;


}




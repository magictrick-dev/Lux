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

static inline b32
symbol_is_numeric(tokenizer *state)
{
    char c = state->source[state->step];
    return (c >= 48 && c <= 57);
}

static inline b32
symbol_is_alpha(tokenizer *state)
{
    char c = state->source[state->step];
    b32 is_lower = (c >= 97 && c <= 122);
    b32 is_upper = (c >= 65 && c <= 90);
    return (is_lower || is_upper);
}

static inline b32
symbol_is_alphanumeric(tokenizer *state)
{
    b32 result = (symbol_is_numeric(state) || symbol_is_alpha(state));
    return result;
}

static inline b32
symbol_is_hexadecimal(tokenizer *state)
{
    
    char c = state->source[state->step];
    b32 is_numeric = symbol_is_numeric(state);
    b32 is_hex = (c >= 97 && c <= 102) || (c >= 65 && c <= 70);
    return (is_numeric || is_hex);

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

        case '=':
        {

            char peek = peek_symbol_at(state, 1);
            if (peek == '=')
            {

                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::EQUALS_EQUALS);
                state->offset = state->step;
                return true;
                
            }

            else
            {

                consume_symbol(state);
                initialize_token(state, token, token_type::EQUALS);
                state->offset = state->step;
                return true;

            }

        } break;

        case '!':
        {

            char peek = peek_symbol_at(state, 1);
            if (peek == '=')
            {

                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::BANG_EQUALS);
                state->offset = state->step;
                return true;
                
            }

            else
            {

                consume_symbol(state);
                initialize_token(state, token, token_type::BANG);
                state->offset = state->step;
                return true;

            }

        } break;

        case '(':
        {

            consume_symbol(state);
            initialize_token(state, token, token_type::LEFT_PARENTHESIS);
            state->offset = state->step;
            return true;

        } break;

        case ')':
        {

            consume_symbol(state);
            initialize_token(state, token, token_type::RIGHT_PARENTHESIS);
            state->offset = state->step;
            return true;

        } break;

        case '[':
        {

            consume_symbol(state);
            initialize_token(state, token, token_type::LEFT_ANGLE_BRACKET);
            state->offset = state->step;
            return true;

        } break;

        case ']':
        {

            consume_symbol(state);
            initialize_token(state, token, token_type::RIGHT_ANGLE_BRACKET);
            state->offset = state->step;
            return true;

        } break;

        case '{':
        {

            consume_symbol(state);
            initialize_token(state, token, token_type::LEFT_CURLY_BRACKET);
            state->offset = state->step;
            return true;

        } break;

        case '}':
        {

            consume_symbol(state);
            initialize_token(state, token, token_type::RIGHT_CURLY_BRACKET);
            state->offset = state->step;
            return true;

        } break;

        case ':':
        {

            char peek = peek_symbol_at(state, 1);
            if (peek == '=')
            {

                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::COLON_EQUALS);
                state->offset = state->step;
                return true;
                
            }

            else
            {

                consume_symbol(state);
                initialize_token(state, token, token_type::COLON);
                state->offset = state->step;
                return true;

            }

        } break;

        case ';':
        {

            consume_symbol(state);
            initialize_token(state, token, token_type::SEMICOLON);
            state->offset = state->step;
            return true;

        } break;

        case '&':
        {

            char peek = peek_symbol_at(state, 1);
            if (peek == '&')
            {

                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::AMPERSAND_AMPERSAND);
                state->offset = state->step;
                return true;
                
            }

            else
            {

                consume_symbol(state);
                initialize_token(state, token, token_type::AMPERSAND);
                state->offset = state->step;
                return true;

            }

        } break;

        case '|':
        {

            char peek = peek_symbol_at(state, 1);
            if (peek == '|')
            {

                consume_symbol(state);
                consume_symbol(state);
                initialize_token(state, token, token_type::PIPE_PIPE);
                state->offset = state->step;
                return true;
                
            }

            else
            {

                consume_symbol(state);
                initialize_token(state, token, token_type::PIPE);
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

            while (!tokenizer_is_eof(state) && !tokenizer_is_eol(state)) 
                consume_symbol(state);

            state->offset = state->step;
            return true;

        } break;

        case '*':
        {

            consume_symbol(state);
            consume_symbol(state);
            
            while (!tokenizer_is_eof(state))
            {

                if (peek_symbol(state) == '*')
                {
                    consume_symbol(state);
                    if (peek_symbol(state) == '/')
                    {
                        consume_symbol(state);

                        state->offset = state->step;
                        return true;
                    }
                }

                consume_symbol(state);
            }

        }

    };

    return false;
}

static inline b32
match_numbers_and_tokenize(tokenizer *state, source_token *token)
{

    if (!symbol_is_numeric(state)) return false;

    // Hexademical case.
    char head = peek_symbol_at(state, 0);
    char marker = peek_symbol_at(state, 1);
    if (head == '0' && (marker == 'x' || marker == 'X'))
    {

        consume_symbol(state);
        consume_symbol(state);

        while (symbol_is_hexadecimal(state))
            consume_symbol(state);

        initialize_token(state, token, token_type::HEXADECIMAL);
        state->offset = state->step;
        return true;

    }

    // Either integer or real.
    uint32_t type = token_type::INTEGER;
    while (true)
    {
        
        consume_symbol(state);

        // If we encounter a decimal, switch type and consume or
        // return true.
        if (peek_symbol(state) == '.')
        {
            if (type == token_type::INTEGER)
            {
                type = token_type::REAL;
                consume_symbol(state);
            }
            else
            {
                break;
            }
        }

        if (!symbol_is_numeric(state))
            break;

    }

    initialize_token(state, token, type);
    state->offset = state->step;
    return true;
}

static inline b32
match_identifiers_and_tokenize(tokenizer *state, source_token *token)
{

    // First character must either be an underscore or alpha.
    char head = peek_symbol(state);
    if (!(symbol_is_alpha(state) || head == '_')) return false;
    consume_symbol(state);

    // All following characters can be any combination of underscores or alnums.
    char current = peek_symbol(state);
    while (current == '_' || symbol_is_alphanumeric(state))
    {
        consume_symbol(state);
        current = peek_symbol(state);
    }

    initialize_token(state, token, token_type::IDENTIFIER);
    state->offset = state->step;

    // TODO(Chris): At this point, we need to check if the token matches an
    //              identifier keyword and switch the token type to the keyword.
    //              Keywords are first-class token types.

    return true;
}

static inline b32
match_strings_and_tokenize(tokenizer *state, source_token *token)
{

    char head = peek_symbol(state);
    char match;
    token_type type;

    switch (head)
    {
        case '\'':
        {
            match = '\'';
            type = token_type::SINGLE_QUOTE_STRING;
        } break;

        case '"':
        {
            match = '"';
            type = token_type::DOUBLE_QUOTE_STRING;
        } break;

        default:
        {
            return false;
        } break;

    }

    consume_symbol(state);
    state->offset = state->step; // We don't actually care about the quotes.

    // Consume until end of line or file or match.
    while (peek_symbol(state) != match && !(tokenizer_is_eol(state) || tokenizer_is_eof(state)))
        consume_symbol(state);

    if (tokenizer_is_eol(state))
    {
        initialize_token(state, token, token_type::UNDEFINED_STR_EOL);
        return true;
    }

    else if (tokenizer_is_eof(state))
    {
        initialize_token(state, token, token_type::UNDEFINED_STR_EOF);
        return true;
    }

    initialize_token(state, token, type);
    consume_symbol(state); // Consume the final quote.
    state->offset = state->step;
    return true;
    
}

b32 
get_next_token(tokenizer *state, source_token *token)
{

    // Clear out non-renderable symbols.
    while (match_whitespace(state, token));
    while (match_comments(state, token));
    while (match_whitespace(state, token));
    state->offset = state->step;

    // If we reached EOF, return EOF token.
    if (tokenizer_is_eof(state)) 
    {
        initialize_token(state, token, token_type::END_OF_FILE);
        return false;
    }
    
    // Match each our cases.
    if (match_symbols_and_tokenize(state, token))       return true;
    if (match_numbers_and_tokenize(state, token))       return true;
    if (match_strings_and_tokenize(state, token))       return true;
    if (match_identifiers_and_tokenize(state, token))   return true;

    // Undefined symbol encountered.
    consume_symbol(state);
    initialize_token(state, token, token_type::UNDEFINED);
    state->offset = state->step;
    return true;

}




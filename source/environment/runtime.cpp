#include <environment/runtime.h>
#include <stdio.h>

typedef struct environment_state
{
    memory_arena heap_arena;
    const char *source_path;
} environment_state;

persist environment_state state;

b32     
environment_runtime_init(environment_arguments *arguments)
{

    // Display the startup banner.
    printf("-------------------------------------------------------------------\n");
    printf("      LUX     : Version 0.1A                 \n");
    printf("  PROGRAMMING : Copyright 2024               \n");
    printf("    LANGUAGE  : Chris DeJong                 \n");
    printf("    COMPILER  : magictrick-dev @ github      \n");
    printf("-------------------------------------------------------------------\n");

    // Push our default allocator.
    allocator_context *default_context = get_default_allocator_context();
    default_context->allocate = clib_malloc;
    default_context->release = clib_free;
    push_allocator_context(default_context);

    // Get our bulk region of memory on startup.
    u64 heap_size = lux_gigabytes(4);
    state.heap_arena.memory = platform_virtual_alloc(NULL, heap_size);
    state.heap_arena.commit = 0;
    if (state.heap_arena.memory.ptr == NULL)
        return environment_return_codes::init_memory_allocation_failure;

    // Check our arguments.
    if (arguments->count < 2) 
    {
        printf("Error: Not enough arguments.\n");
        return environment_return_codes::init_invalid_arguments;
    }

    // Check if the file we passed in is valid.
    if (!platform_file_exists(arguments->list[1]))
    {
        printf("Error: Invalid file path for %s.\n", arguments->list[1]);
        return environment_return_codes::init_invalid_file_path;
    }

    // Set the source path we are working with.
    state.source_path = arguments->list[1];

    return environment_return_codes::init_success;
}

b32     
environment_runtime_main()
{

    // Load the file into memory.
    u64 file_size = platform_get_file_size(state.source_path);
    char *file_buffer = arena_push_array(&state.heap_arena, char, file_size + 1);
    u64 bytes_read = platform_read_entire_file(state.source_path, file_buffer, file_size);
    lux_assert(file_size == bytes_read);
    file_buffer[file_size] = '\0';

    // Once the file is loaded into memory, let us try to lex the file.
    tokenizer tokenizer_state = {};
    tokenizer_state.source = file_buffer;

    source_token current_token = {};
    while (get_next_token(&tokenizer_state, &current_token))
    {
        if (current_token.type == token_type::UNDEFINED)
        {
            printf("UNDEFINED TOKEN ENCOUNTERED.\n");
            const char *offset = current_token.lexeme.source + current_token.lexeme.offset;
            char buffer[256];
            memcpy(buffer, offset, current_token.lexeme.length);
            buffer[current_token.lexeme.length] = '\0';
            printf("    '%s'.\n", buffer);
        }
        else
            printf("Type encountered: %d.\n", current_token.type);
    }

    return environment_return_codes::main_success;
}

void    
environment_runtime_exit(b32 exit_code)
{

    return;
}



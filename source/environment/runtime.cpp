#include <environment/runtime.h>
#include <stdio.h>

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

    return environment_return_codes::init_success;
}

b32     
environment_runtime_main()
{


    return environment_return_codes::main_success;
}

void    
environment_runtime_exit(b32 exit_code)
{

    return;
}



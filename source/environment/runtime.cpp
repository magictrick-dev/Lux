#include <environment/runtime.h>
#include <stdio.h>

persist environment_state state;

b32     
environment_runtime_init(environment_arguments *arguments)
{

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



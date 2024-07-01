#ifndef SOURCE_ENVIRONMENT_RUNTIME_H
#define SOURCE_ENVIRONMENT_RUNTIME_H
#include <core/definitions.h>
#include <core/arena.h>

typedef enum environment_return_codes : b32
{
    init_success,
    init_memory_allocation_failure,
    main_success,
} environment_return_codes;

typedef struct environment_arguments
{
    i32     count;
    char  **list;
} environment_arguments;

typedef struct environment_state
{
    memory_arena heap_arena;
} environment_state;

b32     environment_runtime_init(environment_arguments *arguments);
b32     environment_runtime_main();
void    environment_runtime_exit(b32 exit_code);

#endif

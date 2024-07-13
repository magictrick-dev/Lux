#ifndef SOURCE_CORE_ARENA_H
#define SOURCE_CORE_ARENA_H
#include <core/definitions.h>

typedef struct memory_arena
{
    buffer memory;
    u64 commit;
} memory_arena;

#define arena_push_type(arena, type) (type*)arena_push(arena, sizeof(type))
#define arena_push_array(arena, type, count) (type*)arena_push(arena, (count) * sizeof(type))

inline void*    
arena_push(memory_arena *arena, u64 bytes)
{
    lux_assert(arena->commit + bytes <= arena->memory.size);
    void *alloc_ptr = (u8*)arena->memory.ptr + arena->commit;
    arena->commit += bytes;
    return alloc_ptr;
}

inline void     
arena_pop(memory_arena *arena, u64 bytes)
{
    arena->commit -= bytes;
    lux_assert(arena->commit <= arena->memory.size); // Ensures we didn't pop under zero.
}

inline u64
arena_save(memory_arena *arena)
{
    u64 result = arena->commit;
    return result;
}

inline void
arena_restore(memory_arena *arena, u64 state)
{
    arena->commit = state;
    return;
}

#endif

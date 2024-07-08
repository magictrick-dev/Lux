#ifndef SOURCE_CORE_ALLOCATOR_H
#define SOURCE_CORE_ALLOCATOR_H
#include <core/definitions.h>

typedef buffer  (*memory_allocate_fptr)(u64 size);
typedef void    (*memory_release_fptr)(void* memory);

typedef struct allocator_context 
{
    memory_allocate_fptr allocate;
    memory_release_fptr release;
    allocator_context *parent_context;
    void *user_defined;
} allocator_context;

void*               memory_allocate(u64 size);
void                memory_release(void* pointer);
void                push_allocator_context(allocator_context *context);
allocator_context*  pop_allocator_context();
allocator_context*  get_default_allocator_context();
allocator_context*  get_current_allocator_context();

buffer  clib_malloc(u64 size);
void    clib_free(buffer memory);

#endif

#include <core/allocator.h>

internal allocator_context *current_context;
internal allocator_context default_context;

buffer              
memory_allocate(u64 size)
{
    assert(current_context != NULL);
    buffer result = current_context->allocate(size);
    return result;
}

void                
memory_release(buffer memory)
{

    assert(current_context != NULL);
    current_context->release(memory);
    return;

}

void                
push_allocator_context(allocator_context *context)
{

    assert(context != NULL);
    context->parent_context = current_context;
    current_context = context;
    return;

}

allocator_context*  
pop_allocator_context()
{

    allocator_context *return_context = current_context;
    current_context = return_context->parent_context;
    return return_context;
    
}

allocator_context*  
get_default_allocator_context()
{

    return &default_context;

}

allocator_context*  
get_current_allocator_context()
{

    return current_context;

}

buffer 
clib_malloc(u64 size)
{
    void *memory_ptr = malloc(size);
    buffer result;
    result.ptr = memory_ptr;
    result.size = size;
    return result;
    
}

void    
clib_free(buffer memory)
{
    free(memory.ptr);
}


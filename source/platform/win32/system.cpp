#include <windows.h>
#include <platform/system.h>

buffer
platform_virtual_alloc(void* offset, u64 size)
{

    // NOTE(Chris): We will, for future reference, want to query page size dynamically
    //              and properly adjust from that value, but hardcoding now isn't
    //              an issue since Microsoft has yet (and probably won't) change the
    //              kernel to allow for higher page granularity.
    u64 size_to_boundary = size + (size % 4096);
    void *memory = VirtualAlloc(offset, size_to_boundary, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    buffer result = {};
    result.ptr = memory;
    result.size = size_to_boundary;
    return result;

}

void    
platform_virtual_free(buffer* buff)
{

    VirtualFree(buff->ptr, NULL, MEM_RELEASE);

}


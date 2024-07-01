#ifndef SOURCE_PLATFORM_SYSTEM_H
#define SOURCE_PLATFORM_SYSTEM_H
#include <core/definitions.h>

buffer  platform_virtual_alloc(void* offset, u64 size);
void    platform_virtual_free(buffer *buff);

#endif

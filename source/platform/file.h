#ifndef SOURCE_PLATFORM_FILE_H
#define SOURCE_PLATFORM_FILE_H
#include <core/definitions.h>

b32     platform_file_exists(const char *file_path);
u64     platform_get_file_size(const char *file_path);
u64     platform_read_entire_file(const char *file_path, void *buffer, u32 read_size);

#endif

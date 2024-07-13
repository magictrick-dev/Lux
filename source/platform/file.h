#ifndef SOURCE_PLATFORM_FILE_H
#define SOURCE_PLATFORM_FILE_H
#include <core/definitions.h>

b32     platform_file_exists(const char *file_path);
u64     platform_get_file_size(const char *file_path);
u64     platform_read_entire_file(const char *file_path, void *buffer, u32 read_size);

/*
typedef struct file_stream_context
{
    void   *handle;
    u64     pointer;
    u64     end;
} file_stream_context;

b32     platform_create_read_stream(file_stream_context *context, const char *file_path);
void    platform_close_read_stream(file_stream_context *context, const char *file_path);
u64     platform_read_from_read_stream(file_stream_context *context, u64 read_size, void *buffer, u64 buffer_size);
void    platform_set_read_stream_pointer(file_stream_context *context, u64 offset);
*/

#endif

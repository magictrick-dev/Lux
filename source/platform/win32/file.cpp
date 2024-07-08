#include <windows.h>
#include <platform/file.h>

internal inline HANDLE
win32_create_read_file_handle(const char *file_path)
{
    HANDLE file_handle = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ,
            NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    return file_handle;
}

b32
platform_file_exists(const char *file_path)
{
    DWORD file_attributes = GetFileAttributes(file_path);

    bool result = (file_attributes != INVALID_FILE_ATTRIBUTES && 
         !(file_attributes & FILE_ATTRIBUTE_DIRECTORY));

    return result;
}

u64     
platform_get_file_size(const char *file_path)
{

    HANDLE file_handle = win32_create_read_file_handle(file_path);
    if (file_handle == INVALID_HANDLE_VALUE) return 0;

    LARGE_INTEGER file_size;
    GetFileSizeEx(file_handle, &file_size);
    CloseHandle(file_handle);

    u64 result = (u64)file_size.QuadPart;
    return result;

}

u64
platform_read_entire_file(const char *file_path, void *buffer, u32 read_size)
{

    HANDLE file_handle = win32_create_read_file_handle(file_path);
    if (file_handle == INVALID_HANDLE_VALUE) return 0;

    // We aren't guaranteed to read all the bytes into the buffer, so we need to
    // spin over the amount read until we reach the desired amount.
    DWORD total_bytes_read = 0;
    DWORD bytes_read = 0;
    while (total_bytes_read < read_size)
    {
        ReadFile(file_handle, buffer, read_size - total_bytes_read, &bytes_read, NULL);
        total_bytes_read += bytes_read;
    }

    CloseHandle(file_handle);
    return total_bytes_read;

}



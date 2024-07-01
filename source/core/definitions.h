#ifndef SOURCE_CORE_DEFINITIONS_H
#define SOURCE_CORE_DEFINITIONS_H
#include <stdint.h>
#include <assert.h>

#define lux_kilobytes(n) ((u64)n * 1024)
#define lux_megabytes(n) ((u64)1024 * lux_kilobytes(n))
#define lux_gigabytes(n) ((u64)1024 * lux_megabytes(n))
#define lux_terabytes(n) ((u64)1024 * lux_gigabytes(n))

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef int8_t      i8;
typedef int16_t     i16;
typedef int32_t     i32;
typedef int64_t     i64;
typedef float       f32;
typedef double      f64;
typedef int32_t     b32;
typedef int64_t     b64;

#define lux_assert(expr)        assert((expr))
#define lux_noreach(message)    assert(!(message))
#define lux_noimpl(message)     assert(!(message))

#define internal    static
#define persist     static

typedef struct buffer
{
    void   *ptr;
    u64     size;
} buffer;

#endif

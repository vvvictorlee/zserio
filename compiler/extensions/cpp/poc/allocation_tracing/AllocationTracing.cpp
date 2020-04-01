#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

namespace allocation_tracing
{

#define PRINTF_FIRST(format_, ...) do { \
    static size_t depth = 0; \
    if (++depth == 1) \
        printf((format_), ##__VA_ARGS__); \
    --depth; \
} while (false);

// HACK! In general we don't want any allocations, so it is expected that there will be only few allocations
// and since we need to somehow print the tracing, we simulate allocations at least with 1MB of static buffer
void* alloc_impl(size_t size)
{
    static uint8_t buffer[1*1024*1024]; // 1MB
    static uint8_t* ptr = buffer;
    static const uint8_t* buffer_end = buffer + sizeof(buffer);

    if (ptr + size > buffer_end)
        exit(13);

    void* const rptr = ptr;
    ptr+=size;

    return rptr;
}

void* tracing_malloc(size_t size)
{
    void* const ptr = alloc_impl(size);
    PRINTF_FIRST("malloc: %" PRIu64 " -> %p\n", size, ptr);
    return ptr;
}

void* tracing_calloc(size_t count, size_t el_size)
{
    void* const ptr = alloc_impl(count*el_size);
    PRINTF_FIRST("calloc: %" PRIu64 ", %" PRIu64 " -> %p\n", count, el_size, ptr);
    return ptr;
}

void* tracing_realloc(void* ptr, size_t size)
{
    void* const rptr = alloc_impl(size);
    PRINTF_FIRST("realloc: %p, %" PRIu64 " -> %p\n", ptr, size, rptr);
    return rptr;
}

void tracing_free(void* ptr)
{
    PRINTF_FIRST("free: %p\n", ptr);
}

} // namespace allocation_tracing

void* operator new(size_t size) noexcept(false)
{
    PRINTF_FIRST("new\n");
    return allocation_tracing::tracing_malloc(size);
}

void* operator new[](size_t size) noexcept(false)
{
    PRINTF_FIRST("new[]\n");
    return allocation_tracing::tracing_malloc(size);
}

void operator delete(void* ptr) noexcept
{
    PRINTF_FIRST("delete\n");
    allocation_tracing::tracing_free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    PRINTF_FIRST("delete[]\n");
    allocation_tracing::tracing_free(ptr);
}

extern "C" void * __cxa_allocate_exception(size_t thrown_size) _GLIBCXX_NOTHROW
{
    return allocation_tracing::tracing_malloc(thrown_size);
}

extern "C" void __cxa_free_exception(void *thrown_object) _GLIBCXX_NOTHROW
{
}

extern "C"
{
    void* malloc(size_t size)
    {
        return allocation_tracing::tracing_malloc(size);
    }

    void* calloc(size_t size, size_t el_size)
    {
        return allocation_tracing::tracing_calloc(size, el_size);
    }

    void* realloc(void* ptr, size_t size)
    {
        return allocation_tracing::tracing_realloc(ptr, size);
    }

    void free(void* ptr)
    {
        allocation_tracing::tracing_free(ptr);
    }
}

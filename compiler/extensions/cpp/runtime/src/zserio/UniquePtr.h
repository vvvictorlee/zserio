#ifndef ZSERIO_UNIQUE_PTR_H_INC
#define ZSERIO_UNIQUE_PTR_H_INC

#include <memory>
#include "zserio/RebindAlloc.h"

namespace zserio
{

namespace detail
{

template <typename T, class ALLOC_T>
struct UniquePtrDeleter
{
    template <typename ALLOC_U>
    UniquePtrDeleter(const ALLOC_U& allocator) :
            m_allocator(allocator)
    {}

    void operator()(T* ptr)
    {
        ptr->~T();
        m_allocator.deallocate(ptr, 1);
    }

private:
    ALLOC_T m_allocator;
};

} // namespace detail

template <typename T, typename ALLOC, class ...Args>
std::unique_ptr<T, detail::UniquePtrDeleter<T, RebindAlloc<ALLOC, T>>>
allocate_unique(const ALLOC& allocator, Args&& ...args)
{
    RebindAlloc<ALLOC, T> typedAllocator = allocator;
    return std::unique_ptr<T, detail::UniquePtrDeleter<T, RebindAlloc<ALLOC, T>>>(
            new (typedAllocator.allocate(1)) T(std::forward<Args>(args)...), typedAllocator);
}

} // namespace zserio

#endif // ZSERIO_UNIQUE_PTR_H_INC

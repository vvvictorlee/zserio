#ifndef ZSERIO_UNIQUE_PTR_H_INC
#define ZSERIO_UNIQUE_PTR_H_INC

#include <memory>
#include <type_traits>
#include "zserio/RebindAlloc.h"

namespace zserio
{

namespace detail
{

template <class ALLOC_T>
struct UniquePtrDeleter
{
    typedef typename ALLOC_T::value_type T;

    template <typename ALLOC_U>
    UniquePtrDeleter(const ALLOC_U& allocator) :
            m_allocator(allocator)
    {
        static_assert(std::is_same<ALLOC_T, RebindAlloc<ALLOC_U, T>>::value,
                "UniquePtrDeleter requires same allocator in constructor!");
    }

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
std::unique_ptr<T, detail::UniquePtrDeleter<RebindAlloc<ALLOC, T>>>
allocate_unique(const ALLOC& allocator, Args&& ...args)
{
    RebindAlloc<ALLOC, T> typedAllocator = allocator;
    return std::unique_ptr<T, detail::UniquePtrDeleter<RebindAlloc<ALLOC, T>>>(
            new (typedAllocator.allocate(1)) T(std::forward<Args>(args)...), typedAllocator);
}

} // namespace zserio

#endif // ZSERIO_UNIQUE_PTR_H_INC

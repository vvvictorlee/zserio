#ifndef ZSERIO_PMR_UNIQUE_PTR_H_INC
#define ZSERIO_PMR_UNIQUE_PTR_H_INC

#include <memory>
#include "zserio/pmr/PolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

namespace detail
{

template <typename T>
struct PolymorphicDeleter
{
    template <typename U>
    PolymorphicDeleter(const PolymorphicAllocator<U>& allocator) :
            m_allocator(allocator)
    {}

    void operator()(T* ptr)
    {
        ptr->~T();
        m_allocator.deallocate(ptr, 1);
    }

private:
    PolymorphicAllocator<T> m_allocator;
};

} // namespace detail

template <typename T>
using unique_ptr = std::unique_ptr<T, detail::PolymorphicDeleter<T>>;

template <typename T, typename U, class ...Args>
unique_ptr<T> allocate_unique(const PolymorphicAllocator<U>& allocator, Args&& ...args)
{
    PolymorphicAllocator<T> typeAllocator = allocator;
    return unique_ptr<T>(new (typeAllocator.allocate(1)) T(std::forward<Args>(args)...), typeAllocator);
}

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_UNIQUE_PTR_H_INC

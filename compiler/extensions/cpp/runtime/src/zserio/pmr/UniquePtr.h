#ifndef ZSERIO_PMR_UNIQUE_PTR_H_INC
#define ZSERIO_PMR_UNIQUE_PTR_H_INC

#include "zserio/UniquePtr.h"
#include "zserio/pmr/PolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

template <typename T>
using unique_ptr = std::unique_ptr<T, detail::UniquePtrDeleter<T, PolymorphicAllocator<T>>>;

template <typename T, typename U, class ...Args>
unique_ptr<T> allocate_unique(const PolymorphicAllocator<U>& allocator, Args&& ...args)
{
    return zserio::allocate_unique<T>(allocator, std::forward<Args>(args)...);
}

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_UNIQUE_PTR_H_INC

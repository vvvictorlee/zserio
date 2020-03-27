#ifndef ZSERIO_PMR_POLYMORPHIC_ALLOCATOR_H_INC
#define ZSERIO_PMR_POLYMORPHIC_ALLOCATOR_H_INC

#include "zserio/pmr/MemoryResource.h"

namespace zserio
{
namespace pmr
{

template <class T>
class PolymorphicAllocator
{
public:
    using value_type = T;

    PolymorphicAllocator(MemoryResource& resource) noexcept
    :   m_resource(resource)
    {}

    template <class U>
    PolymorphicAllocator(const PolymorphicAllocator<U>& alloc) noexcept
    :   m_resource(alloc.resource())
    {}

    value_type* allocate(std::size_t n)
    {
        return static_cast<value_type*>(m_resource.allocate(n * sizeof(value_type), alignof(value_type)));
    }

    void deallocate(value_type* p, std::size_t n) noexcept
    {
        m_resource.deallocate(p, n*sizeof(value_type), alignof(value_type));
    }

    MemoryResource& resource() const noexcept
    {
        return m_resource;
    }

private:
    MemoryResource& m_resource;
};

template <class T, class U>
bool operator==(const PolymorphicAllocator<T>& lhs, const PolymorphicAllocator<U>& rhs) noexcept
{
    return lhs.resource() == rhs.resource();
}

template <class T, class U>
bool operator!=(const PolymorphicAllocator<T>& lhs, const PolymorphicAllocator<U>& rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_POLYMORPHIC_ALLOCATOR_H_INC

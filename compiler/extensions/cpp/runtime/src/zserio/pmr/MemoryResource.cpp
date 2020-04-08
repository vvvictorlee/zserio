#include "zserio/pmr/MemoryResource.h"

#include "zserio/pmr/NewDeleteResource.h"

namespace zserio
{
namespace pmr
{

MemoryResource& getDefaultResource() noexcept
{
    return getNewDeleteResource();
}

} // namespace pmr
} // namespace zserio

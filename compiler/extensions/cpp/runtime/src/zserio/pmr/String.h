#ifndef ZSERIO_PMR_STRING_H_INC
#define ZSERIO_PMR_STRING_H_INC

#include <string>
#include "zserio/pmr/PolymorphicAllocator.h"

namespace zserio
{
namespace pmr
{

template <typename CHAR, typename TRAITS = std::char_traits<CHAR>>
using basic_string = std::basic_string<CHAR, TRAITS, PolymorphicAllocator<CHAR>>;

using string = basic_string<char>;

} // namespace pmr
} // namespace zserio

#endif // ZSERIO_PMR_STRING_H_INC

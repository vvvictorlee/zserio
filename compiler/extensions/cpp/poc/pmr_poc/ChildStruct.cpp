/**
 * Automatically generated by Zserio C++ extension version 2.0.0-pre1.
 */

#include <zserio/StringConvertUtil.h>
#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/BitPositionUtil.h>
#include <zserio/BitSizeOfCalculator.h>
#include <zserio/BitFieldUtil.h>

#include <pmr_poc/ChildStruct.h>

namespace pmr_poc
{

ChildStruct::ChildStruct(::zserio::BitStreamReader& in,
        const ::zserio::pmr::PolymorphicAllocator<void>& allocator) :
        m_uint64Field_(readUint64Field(in)),
        m_uint16Array_(readUint16Array(in, allocator))
{
}

uint64_t ChildStruct::getUint64Field() const
{
    return m_uint64Field_;
}

const ::zserio::pmr::vector<uint16_t>& ChildStruct::getUint16Array() const
{
    return m_uint16Array_;
}

size_t ChildStruct::bitSizeOf(size_t bitPosition) const
{
    size_t endBitPosition = bitPosition;

    endBitPosition += UINT8_C(64);
    endBitPosition += ::zserio::bitSizeOfAuto(
            ::zserio::StdIntArrayTraits<uint16_t>(), m_uint16Array_, endBitPosition);

    return endBitPosition - bitPosition;
}

bool ChildStruct::operator==(const ChildStruct& other) const
{
    if (this != &other)
    {
        return
                (m_uint64Field_ == other.m_uint64Field_) &&
                (m_uint16Array_ == other.m_uint16Array_);
    }

    return true;
}

int ChildStruct::hashCode() const
{
    int result = ::zserio::HASH_SEED;

    result = ::zserio::calcHashCode(result, m_uint64Field_);
    result = ::zserio::calcHashCode(result, m_uint16Array_);

    return result;
}

uint64_t ChildStruct::readUint64Field(::zserio::BitStreamReader& in)
{
    return static_cast<uint64_t>(in.readBits64(UINT8_C(64)));
}

::zserio::pmr::vector<uint16_t> ChildStruct::readUint16Array(::zserio::BitStreamReader& in,
        const ::zserio::pmr::PolymorphicAllocator<void>& allocator)
{
    ::zserio::pmr::vector<uint16_t> readField{allocator};
    ::zserio::readAuto(::zserio::StdIntArrayTraits<uint16_t>(), readField, in);
    return readField;
}

} // namespace pmr_poc

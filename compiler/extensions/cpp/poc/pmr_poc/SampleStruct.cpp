/**
 * Automatically generated by Zserio C++ extension version 2.0.0-pre1.
 */

#include <zserio/StringConvertUtil.h>
#include <zserio/CppRuntimeException.h>
#include <zserio/HashCodeUtil.h>
#include <zserio/BitPositionUtil.h>
#include <zserio/BitSizeOfCalculator.h>
#include <zserio/BitFieldUtil.h>
#include <zserio/BitBuffer.hpp>

#include <pmr_poc/SampleStruct.h>

namespace pmr_poc
{

class SampleStruct::ElementFactory_childField
{
public:
    explicit ElementFactory_childField(SampleStruct& owner) : m_owner(owner) {}

    void create(::zserio::pmr::vector<::pmr_poc::ChildStruct>& array, ::zserio::BitStreamReader& in, size_t index) const
    {
        (void)index;
        array.emplace_back(in, array.get_allocator());
    }

private:
    SampleStruct& m_owner;
};

SampleStruct::SampleStruct(::zserio::BitStreamReader& in,
        const ::zserio::pmr::PolymorphicAllocator<void>& allocator) :
        m_uint16Field_(readUint16Field(in)),
        m_stringField_(readStringField(in, allocator)),
        m_externField_(readExternField(in, allocator)),
        m_inplaceOptionalField_(readInplaceOptionalField(in)),
        m_heapOptionalField_(readHeapOptionalField(in, allocator)),
        m_childField_(readChildField(in, allocator))
{
}

uint16_t SampleStruct::getUint16Field() const
{
    return m_uint16Field_;
}

const ::zserio::pmr::string& SampleStruct::getStringField() const
{
    return m_stringField_;
}

const ::zserio::pmr::BitBuffer& SampleStruct::getExternField() const
{
    return m_externField_;
}

const ::pmr_poc::BigStruct& SampleStruct::getInplaceOptionalField() const
{
    return m_inplaceOptionalField_.value();
}

bool SampleStruct::hasInplaceOptionalField() const
{
    return (m_inplaceOptionalField_.hasValue());
}

const ::pmr_poc::StringStruct& SampleStruct::getHeapOptionalField() const
{
    return *m_heapOptionalField_;
}

bool SampleStruct::hasHeapOptionalField() const
{
    return static_cast<bool>(m_heapOptionalField_);
}

const ::zserio::pmr::vector<::pmr_poc::ChildStruct>& SampleStruct::getChildField() const
{
    return m_childField_;
}

size_t SampleStruct::bitSizeOf(size_t bitPosition) const
{
    size_t endBitPosition = bitPosition;

    endBitPosition += UINT8_C(16);
    endBitPosition += ::zserio::bitSizeOfString(m_stringField_);
    endBitPosition += ::zserio::bitSizeOfBitBuffer(m_externField_);
    endBitPosition += 1;
    if (m_inplaceOptionalField_.hasValue())
    {
        endBitPosition += m_inplaceOptionalField_.value().bitSizeOf(endBitPosition);
    }
    endBitPosition += 1;
    if (m_heapOptionalField_)
    {
        endBitPosition += m_heapOptionalField_->bitSizeOf(endBitPosition);
    }
    endBitPosition += ::zserio::bitSizeOfAuto(
            ::zserio::ObjectArrayTraits<::pmr_poc::ChildStruct>(), m_childField_, endBitPosition);

    return endBitPosition - bitPosition;
}

bool SampleStruct::operator==(const SampleStruct& other) const
{
    if (this != &other)
    {
        return
                (m_uint16Field_ == other.m_uint16Field_) &&
                (m_stringField_ == other.m_stringField_) &&
                (m_externField_ == other.m_externField_) &&
                (m_inplaceOptionalField_ == other.m_inplaceOptionalField_) &&
                (m_heapOptionalField_ == other.m_heapOptionalField_) && // TODO[Mi-L@]: Check!
                (m_childField_ == other.m_childField_);
    }

    return true;
}

int SampleStruct::hashCode() const
{
    int result = ::zserio::HASH_SEED;

    result = ::zserio::calcHashCode(result, m_uint16Field_);
    result = ::zserio::calcHashCode(result, m_stringField_);
    result = ::zserio::calcHashCode(result, m_externField_);
    result = ::zserio::calcHashCode(result, m_inplaceOptionalField_);
    result = ::zserio::calcHashCode(result, m_heapOptionalField_);
    result = ::zserio::calcHashCode(result, m_childField_);

    return result;
}

uint16_t SampleStruct::readUint16Field(::zserio::BitStreamReader& in)
{
    return static_cast<uint16_t>(in.readBits(UINT8_C(16)));
}

::zserio::pmr::string SampleStruct::readStringField(::zserio::BitStreamReader& in,
        const ::zserio::pmr::PolymorphicAllocator<void>& allocator)
{
    return in.readString(allocator);
}

::zserio::pmr::BitBuffer SampleStruct::readExternField(::zserio::BitStreamReader& in,
        const ::zserio::pmr::PolymorphicAllocator<void>& allocator)
{
    return static_cast<::zserio::pmr::BitBuffer>(in.readBitBuffer(allocator));
}

::zserio::OptionalHolder<::pmr_poc::BigStruct> SampleStruct::readInplaceOptionalField(
        ::zserio::BitStreamReader& in)
{
    if (in.readBool())
        return ::pmr_poc::BigStruct(in);

    return ::zserio::NullOpt;
}

::zserio::pmr::unique_ptr<::pmr_poc::StringStruct> SampleStruct::readHeapOptionalField(
        ::zserio::BitStreamReader& in, const ::zserio::pmr::PolymorphicAllocator<void>& allocator)
{
    if (in.readBool())
    {
        return zserio::pmr::allocate_unique<::pmr_poc::StringStruct>(allocator, in, allocator);
    }

    return ::zserio::pmr::unique_ptr<::pmr_poc::StringStruct>(nullptr, allocator);
}

::zserio::pmr::vector<::pmr_poc::ChildStruct> SampleStruct::readChildField(::zserio::BitStreamReader& in,
        const ::zserio::pmr::PolymorphicAllocator<void>& allocator)
{
    ::zserio::pmr::vector<::pmr_poc::ChildStruct> readField{allocator};
    ::zserio::readAuto(
            ::zserio::ObjectArrayTraits<::pmr_poc::ChildStruct, ElementFactory_childField>(
                    ElementFactory_childField(*this)),
            readField, in);
    return readField;
}

} // namespace pmr_poc

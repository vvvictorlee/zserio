/**
 * Automatically generated by Zserio C++ extension version 2.0.0-pre1.
 */

#ifndef PMR_POC_STRING_STRUCT_H
#define PMR_POC_STRING_STRUCT_H

#include <zserio/BitStreamReader.h>
#include <zserio/BitStreamWriter.h>
#include <zserio/PreWriteAction.h>
#include <zserio/pmr/String.h>

namespace pmr_poc
{

class StringStruct
{
public:
    explicit StringStruct(::zserio::BitStreamReader& in,
            const ::zserio::pmr::PolymorphicAllocator<void>& allocator =
                    ::zserio::pmr::PolymorphicAllocator<void>());

    ~StringStruct() = default;

    StringStruct(const StringStruct&) = default;
    StringStruct& operator=(const StringStruct&) = default;

    StringStruct(StringStruct&&) = default;
    StringStruct& operator=(StringStruct&&) = default;

    const ::zserio::pmr::string& getStringField() const;

    size_t bitSizeOf(size_t bitPosition = 0) const;

    bool operator==(const StringStruct& other) const;
    int hashCode() const;

private:
    ::zserio::pmr::string readStringField(::zserio::BitStreamReader& in,
            const ::zserio::pmr::PolymorphicAllocator<void>& allocator);

    ::zserio::pmr::string m_stringField_;
};

} // namespace pmr_poc

#endif // PMR_POC_STRING_STRUCT_H
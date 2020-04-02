#ifndef ZSERIO_BIT_BUFFER_HPP_INC
#define ZSERIO_BIT_BUFFER_HPP_INC

#include <utility>
#include <cstring>

#include "zserio/HashCodeUtil.h"
#include "zserio/CppRuntimeException.h"
#include "zserio/StringConvertUtil.h"

namespace zserio
{

namespace detail
{

template <typename ALLOC>
BitBuffer<ALLOC>::BitBuffer(const ALLOC& allocator) :
        m_buffer(allocator), m_bitSize(0)
{
}

template <typename ALLOC>
BitBuffer<ALLOC>::BitBuffer(size_t bitSize, const ALLOC& allocator) :
        m_buffer((bitSize + 7) / 8, allocator), m_bitSize(bitSize)
{
}

template <typename ALLOC>
BitBuffer<ALLOC>::BitBuffer(const std::vector<uint8_t, ALLOC>& buffer) :
        m_buffer(buffer), m_bitSize(8 * buffer.size())
{
}

template <typename ALLOC>
BitBuffer<ALLOC>::BitBuffer(const std::vector<uint8_t, ALLOC>& buffer, size_t bitSize) :
        m_buffer(buffer.get_allocator()), m_bitSize(bitSize)
{
    const size_t byteSize = (bitSize + 7) / 8;
    if (buffer.size() < byteSize)
        throw CppRuntimeException("BitBuffer: Bit size " + convertToString(bitSize) +
                " out of range for given vector byte size " + convertToString(buffer.size()) + "!");

    m_buffer.assign(buffer.data(), buffer.data() + byteSize);
}

template <typename ALLOC>
BitBuffer<ALLOC>::BitBuffer(std::vector<uint8_t, ALLOC>&& buffer) :
        m_buffer(std::move(buffer)), m_bitSize(8 * m_buffer.size())
{
}

template <typename ALLOC>
BitBuffer<ALLOC>::BitBuffer(std::vector<uint8_t, ALLOC>&& buffer, size_t bitSize) :
        m_buffer(std::move(buffer)), m_bitSize(bitSize)
{
    const size_t byteSize = (bitSize + 7) / 8;
    if (m_buffer.size() < byteSize)
        throw CppRuntimeException("BitBuffer: Bit size " + convertToString(bitSize) +
                " out of range for given vector byte size " + convertToString(buffer.size()) + "!");
}

template <typename ALLOC>
BitBuffer<ALLOC>::BitBuffer(const uint8_t* buffer, size_t bitSize, const ALLOC& allocator) :
        m_buffer(buffer, buffer + (bitSize + 7) / 8, allocator), m_bitSize(bitSize)
{
}

template <typename ALLOC>
bool BitBuffer<ALLOC>::operator==(const BitBuffer<ALLOC>& other) const
{
    if (this != &other)
    {
        if (m_bitSize != other.m_bitSize)
            return false;

        const size_t byteSize = getByteSize();
        if (byteSize > 0)
        {
            if (byteSize > 1)
            {
                if (memcmp(getBuffer(), other.getBuffer(), byteSize - 1) != 0)
                    return false;
            }

            if (getMaskedLastByte() != other.getMaskedLastByte())
                return false;
        }
    }

    return true;
}

template <typename ALLOC>
int BitBuffer<ALLOC>::hashCode() const
{
    int result = ::zserio::HASH_SEED;
    const size_t byteSize = getByteSize();
    if (byteSize > 0)
    {
        if (byteSize > 1)
        {
            const uint8_t* lastElement = getBuffer() + byteSize - 1;
            for (const uint8_t* p = getBuffer(); p < lastElement; p++)
                result = calcHashCode(result, *p);
        }
        result = ::zserio::calcHashCode(result, getMaskedLastByte());
    }

    return result;
}

template <typename ALLOC>
const uint8_t* BitBuffer<ALLOC>::getBuffer() const
{
    return m_buffer.data();
}

template <typename ALLOC>
uint8_t* BitBuffer<ALLOC>::getBuffer()
{
    return m_buffer.data();
}

template <typename ALLOC>
size_t BitBuffer<ALLOC>::getBitSize() const
{
    return m_bitSize;
}

template <typename ALLOC>
size_t BitBuffer<ALLOC>::getByteSize() const
{
    return (m_bitSize + 7) / 8;
}

template <typename ALLOC>
uint8_t BitBuffer<ALLOC>::getMaskedLastByte() const
{
    const size_t roundedByteSize = m_bitSize / 8;
    const uint8_t lastByteBits = static_cast<uint8_t>(m_bitSize - 8 * roundedByteSize);

    return (lastByteBits == 0) ? m_buffer[roundedByteSize - 1] :
            (m_buffer[roundedByteSize] & (0xFF >> (8 - lastByteBits)));
}

} // namespace detail

} // namespace zserio

#endif // ZSERIO_BIT_BUFFER_HPP_INC

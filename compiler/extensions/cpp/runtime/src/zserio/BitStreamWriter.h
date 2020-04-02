#ifndef ZSERIO_BIT_STREAM_WRITER_H_INC
#define ZSERIO_BIT_STREAM_WRITER_H_INC

#include <cstddef>
#include <cstring>
#include <string>
#include <vector>

#include "zserio/Types.h"
#include "zserio/BitBuffer.h"

namespace zserio
{

/**
 * Writer class which allows to write various data to the bit stream.
 */
class BitStreamWriter
{
public:
    /** Type for bit position. */
    typedef size_t BitPosType;

    /**
     * Constructor.
     *
     * Constructs writer with internally allocated buffer.
     */
    BitStreamWriter();

    /**
     * Constructor from externally allocated byte buffer.
     *
     * \param buffer External byte buffer to create from.
     * \param bufferByteSize Size of the buffer in bytes.
     */
    explicit BitStreamWriter(uint8_t* buffer, size_t bufferByteSize);

    /**
     * Constructor from externally allocated bit buffer.
     *
     * \param bitBuffer External bit buffer to create from.
     */
    template <typename ALLOC>
    explicit BitStreamWriter(detail::BitBuffer<ALLOC>& bitBuffer):
            BitStreamWriter(bitBuffer.getBuffer(), bitBuffer.getBitSize())
    {
    }

    /**
     * Destructor.
     */
    ~BitStreamWriter();

    /**
     * Copying and moving is disallowed!
     * \{
     */
    BitStreamWriter(const BitStreamWriter&) = delete;
    BitStreamWriter& operator=(const BitStreamWriter&) = delete;

    BitStreamWriter(const BitStreamWriter&&) = delete;
    BitStreamWriter& operator=(BitStreamWriter&&) = delete;
    /**
     * \}
     */

    /**
     * Writes unsigned bits up to 32 bits.
     *
     * \param data Data to write.
     * \param numBits Number of bits to write.
     */
    void writeBits(uint32_t data, uint8_t numBits = 32);
    /**
     * Writes unsigned bits up to 64 bits.
     *
     * \param data Data to write.
     * \param numBits Number of bits to write.
     */
    void writeBits64(uint64_t data, uint8_t numBits = 64);

    /**
     * Writes signed bits up to 32 bits.
     *
     * \param data Data to write.
     * \param numBits Number of bits to write.
     */
    void writeSignedBits(int32_t data, uint8_t numBits = 32);
    /**
     * Writes signed bits up to 64 bits.
     *
     * \param data Data to write.
     * \param numBits Number of bits to write.
     */
    void writeSignedBits64(int64_t data, uint8_t numBits = 64);

    /**
     * Writes signed variable integer up to 64 bits.
     *
     * \param data Varint64 to write.
     */
    void writeVarInt64(int64_t data);
    /**
     * Writes signed variable integer up to 32 bits.
     *
     * \param data Varint32 to write.
     */
    void writeVarInt32(int32_t data);
    /**
     * Writes signed variable integer up to 16 bits.
     *
     * \param data Varint16 to write.
     */
    void writeVarInt16(int16_t data);

    /**
     * Writes unsigned variable integer up to 64 bits.
     *
     * \param data Varuint64 to write.
     */
    void writeVarUInt64(uint64_t data);
    /**
     * Writes unsigned variable integer up to 32 bits.
     *
     * \param data Varuint32 to write.
     */
    void writeVarUInt32(uint32_t data);
    /**
     * Writes unsigned variable integer up to 16 bits.
     *
     * \param data Varuint16 to write.
     */
    void writeVarUInt16(uint16_t data);

    /**
     * Writes signed variable integer up to 72 bits.
     *
     * \param data Varuint64 to write.
     */
    void writeVarInt(int64_t data);
    /**
     * Writes signed variable integer up to 72 bits.
     *
     * \param data Varuint64 to write.
     */
    void writeVarUInt(uint64_t data);

    /**
     * Writes 16-bit float.
     *
     * \param data Float16 to write.
     */
    void writeFloat16(float data);
    /**
     * Writes 32-bit float.
     *
     * \param data Float32 to write.
     */
    void writeFloat32(float data);
    /**
     * Writes 64-bit float.
     *
     * \param data Float64 to write.
     */
    void writeFloat64(double data);

    /**
     * Writes UTF-8 string.
     *
     * \param data String to write.
     */
    template <typename ALLOC>
    void writeString(const std::basic_string<char, std::char_traits<char>, ALLOC>& data)
    {
        const size_t len = data.size();
        BitStreamWriter::writeVarUInt64(len);
        for (size_t i = 0; i < len; ++i)
        {
            BitStreamWriter::writeBits(static_cast<uint8_t>(data[i]), 8);
        }
    }

    /**
     * Writes bool as a single bit.
     *
     * \param data Bool to write.
     */
    void writeBool(bool data);

    /**
     * Writes bit buffer.
     *
     * \param bitBuffer Bit buffer to write.
     */
    template <typename ALLOC>
    void writeBitBuffer(const detail::BitBuffer<ALLOC>& bitBuffer)
    {
        const size_t bitSize = bitBuffer.getBitSize();
        writeVarUInt64(bitSize);

        const uint8_t* buffer = bitBuffer.getBuffer();
        size_t numBytesToWrite = bitSize / 8;
        const uint8_t numRestBits = static_cast<uint8_t>(bitSize - numBytesToWrite * 8);
        const BitPosType beginBitPosition = getBitPosition();
        if ((beginBitPosition & 0x07) != 0)
        {
            // we are not aligned to byte
            while (numBytesToWrite > 0)
            {
                writeUnsignedBits(*buffer, 8);
                buffer++;
                numBytesToWrite--;
            }
        }
        else
        {
            // we are aligned to byte
            setBitPosition(beginBitPosition + numBytesToWrite * 8);
            memcpy(m_buffer + beginBitPosition / 8, buffer, numBytesToWrite);
            buffer += numBytesToWrite;
        }

        if (numRestBits > 0)
            writeUnsignedBits(*buffer, numRestBits);
    }

    /**
     * Gets current bit position.
     *
     * \return Current bit position.
     */
    BitPosType getBitPosition() const { return m_bitIndex; }

    /**
     * Sets current bit position. Use with caution!
     *
     * \param pos New bit position.
     */
    void setBitPosition(BitPosType pos);

    /**
     * Moves current bit position to perform the requested bit alignment.
     *
     * \param alignment Size of the alignment in bits.
     */
    void alignTo(size_t alignment);

    /**
     * Gets whether the writer has assigned a write buffer.
     *
     * \return True when a buffer is assigned. False otherwise.
     */
    bool hasWriteBuffer() const { return m_hasInternalBuffer || m_buffer != NULL; }
    /**
     * Gets the write buffer.
     *
     * \param writeBufferByteSize Output parameter which will be set to the write buffer byte size.
     *
     * \return Pointer to the beginning of write buffer.
     */
    const uint8_t* getWriteBuffer(size_t& writeBufferByteSize) const;

    /**
     * Writes the underlying write buffer to file.
     *
     * \param filename Filename of the file to write.
     */
    void writeBufferToFile(const std::string& filename) const;

private:
    void writeUnsignedBits(uint32_t data, uint8_t numBits);
    void writeUnsignedBits64(uint64_t data, uint8_t numBits);
    void writeVarNum(int64_t value, const uint8_t* valBits, size_t valBitsSize, size_t numVarBits);
    void writeVarAbsNum(uint64_t value, bool sign, const uint8_t* valBits, size_t valBitsSize,
            size_t numVarBits);

    uint8_t* m_buffer;
    size_t m_bitIndex;
    size_t m_bufferBitSize;
    bool m_hasInternalBuffer;
    std::vector<uint8_t> m_internalBuffer;
};

} // namespace zserio

#endif // ifndef ZSERIO_BIT_STREAM_WRITER_H_INC

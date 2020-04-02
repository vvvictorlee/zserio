#ifndef ZSERIO_BIT_STREAM_READER_H_INC
#define ZSERIO_BIT_STREAM_READER_H_INC

#include <cstddef>
#include <cstring>
#include <string>

#include "zserio/Types.h"
#include "zserio/BitBuffer.h"
#include "zserio/VarUInt64Util.h"
#include "zserio/RebindAlloc.h"

namespace zserio
{

/**
 * Reader class which allows to read various data from the bit stream.
 */
class BitStreamReader
{
public:
    /** Type for bit position. */
    typedef size_t BitPosType;

    /**
     * Context of the reader defining its state.
     */
    struct ReaderContext
    {
        /**
         * Constructor.
         *
         * \param buffer Pointer to the buffer to read.
         * \param bufferBitSize Size of the buffer in bits.
         */
        explicit ReaderContext(const uint8_t* buffer, size_t bufferBitSize);

        /**
         * Constructor from the file name.
         *
         * \param filename Filename of the binary file to read.
         */
        explicit ReaderContext(const std::string& filename);

        /**
         * Destructor.
         */
        ~ReaderContext();

        /**
         * Copying and moving is disallowed!
         * \{
         */
        ReaderContext(const ReaderContext&) = delete;
        ReaderContext& operator=(const ReaderContext&) = delete;

        ReaderContext(const ReaderContext&&) = delete;
        ReaderContext& operator=(const ReaderContext&&) = delete;
        /**
         * \}
         */

        /** Cache buffer union to cover both 32bit and 64bit implementations. */
        union BitCache
        {
            uint32_t buffer32; /**< Cache buffer used on 32bit platforms. */
            uint64_t buffer64; /**< Cache buffer used on 64bit platforms. */
        };

        uint8_t* buffer; /**< Buffer to read from. */
        BitPosType bufferBitSize; /**< Size of the buffer in bits. */
        bool hasInternalBuffer; /**< Whether the reader has internal buffer. True when reading from file. */

        BitCache cache; /**< Bit cache to optimize bit reading. */
        uint8_t cacheNumBits; /**< Num bits available in the bit cache. */

        BitPosType bitIndex; /**< Current bit index. */

    private:
        void Init();
    };

    /**
     * Constructor from raw buffer.
     *
     * \param buffer Pointer to the buffer to read.
     * \param bufferByteSize Size of the buffer in bytes.
     */
    explicit BitStreamReader(const uint8_t* buffer, size_t bufferByteSize);

    /**
     * Constructor from bit buffer.
     *
     * \param buffer Bit buffer to read from.
     */
    template <typename ALLOC>
    explicit BitStreamReader(const detail::BitBuffer<ALLOC>& bitBuffer) :
            m_context(bitBuffer.getBuffer(), bitBuffer.getBitSize())
    {}

    /**
     * Constructor from the file name.
     *
     * \param filename Filename of the binary file to read.
     */
    explicit BitStreamReader(const std::string& filename);

    /**
     * Destructor.
     */
    ~BitStreamReader();

    /**
     * Reads unsigned bits up to 32-bits.
     *
     * \param numBits Number of bits to read.
     *
     * \return Read bits.
     */
    uint32_t readBits(uint8_t numBits = 32);
    /**
     * Reads unsigned bits up to 64-bits.
     *
     * \param numBits Number of bits to read.
     *
     * \return Read bits.
     */
    uint64_t readBits64(uint8_t numBits = 64);

    /**
     * Reads signed bits up to 32-bits.
     *
     * \param numBits Number of bits to read.
     *
     * \return Read bits.
     */
    int32_t readSignedBits(uint8_t numBits = 32);
    /**
     * Reads signed bits up to 64-bits.
     *
     * \param numBits Number of bits to read.
     *
     * \return Read bits.
     */
    int64_t readSignedBits64(uint8_t numBits = 64);

    /**
     * Reads signed variable integer up to 64 bits.
     *
     * \return Read varint64.
     */
    int64_t readVarInt64();
    /**
     * Reads signed variable integer up to 32 bits.
     *
     * \return Read varint32.
     */
    int32_t readVarInt32();
    /**
     * Reads signed variable integer up to 16 bits.
     *
     * \return Read varint16.
     */
    int16_t readVarInt16();

    /**
     * Read unsigned variable integer up to 64 bits.
     *
     * \return Read varuint64.
     */
    uint64_t readVarUInt64();
    /**
     * Read unsigned variable integer up to 32 bits.
     *
     * \return Read varuint32.
     */
    uint32_t readVarUInt32();
    /**
     * Read unsigned variable integer up to 16 bits.
     *
     * \return Read varuint16.
     */
    uint16_t readVarUInt16();

    /**
     * Reads signed variable integer up to 72 bits.
     *
     * \return Read varint.
     */
    int64_t readVarInt();
    /**
     * Read unsigned variable integer up to 72 bits.
     *
     * \return Read varuint.
     */
    uint64_t readVarUInt();

    /**
     * Reads 16-bit float.
     *
     * \return Read float16.
     */
    float readFloat16();
    /**
     * Reads 32-bit float.
     *
     * \return Read float32.
     */
    float readFloat32();
    /**
     * Reads 64-bit float double).
     *
     * \return Read float64.
     */
    double readFloat64();

    /**
     * Reads an UTF-8 string.
     *
     * \return Read string.
     */
    template <typename ALLOC>
    std::basic_string<char, std::char_traits<char>, zserio::RebindAlloc<ALLOC, char>> readString(
            const ALLOC& alloc)
    {
        std::basic_string<char, std::char_traits<char>, zserio::RebindAlloc<ALLOC, char>> value{alloc};
        const size_t len = convertVarUInt64ToArraySize(readVarUInt64());
        value.reserve(len);
        for (size_t i = 0; i < len; ++i)
        {
            value.push_back(readChar());
        }
        return value;
    }

    /**
     * Reads bool as a single bit.
     *
     * \return Read bool value.
     */
    bool readBool();

    /**
     * Reads a bit buffer.
     *
     * \return Read bit buffer.
     */
    template <typename ALLOC>
    detail::BitBuffer<RebindAlloc<ALLOC, uint8_t>> readBitBuffer(const ALLOC& allocator)
    {
        const size_t bitSize = convertVarUInt64ToArraySize(readVarUInt64());
        size_t numBytesToRead = bitSize / 8;
        const uint8_t numRestBits = static_cast<uint8_t>(bitSize - numBytesToRead * 8);
        detail::BitBuffer<RebindAlloc<ALLOC, uint8_t>> bitBuffer(bitSize, allocator);
        uint8_t* buffer = bitBuffer.getBuffer();
        const BitPosType beginBitPosition = getBitPosition();
        if ((beginBitPosition & 0x07) != 0)
        {
            // we are not aligned to byte
            while (numBytesToRead > 0)
            {
                *buffer = static_cast<uint8_t>(readBits(8));
                buffer++;
                numBytesToRead--;
            }
        }
        else
        {
            // we are aligned to byte
            setBitPosition(beginBitPosition + numBytesToRead * 8);
            memcpy(buffer, m_context.buffer + beginBitPosition / 8, numBytesToRead);
            buffer += numBytesToRead;
        }

        if (numRestBits > 0)
            *buffer = static_cast<uint8_t>(readBits(numRestBits));

        return bitBuffer;
    }

    /**
     * Gets current bit position.
     *
     * \return Current bit position.
     */
    BitPosType getBitPosition() const { return m_context.bitIndex; }

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
     * Gets size of the underlying buffer in bits.
     *
     * \return Buffer bit size.
     */
    size_t getBufferBitSize() const { return m_context.bufferBitSize; }

private:
    uint8_t readChar();

    ReaderContext m_context;
};

} // namespace zserio

#endif // ifndef ZSERIO_BIT_STREAM_READER_H_INC

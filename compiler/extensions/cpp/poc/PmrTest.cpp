#include <limits>
#include <algorithm>
#include <memory>

#include "zserio/Arrays.h"
#include "zserio/pmr/Vector.h"
#include "zserio/BitBuffer.hpp"
#include "pmr_poc/SampleStruct.h"

template <size_t BUFFER_SIZE=1024>
class TestResource : public zserio::pmr::MemoryResource
{
public:
    TestResource()
    {
        std::fill(m_buffer, m_buffer + BUFFER_SIZE, 0);
    }

    ~TestResource()
    {
#if ZSERIO_MEMORY_RESOURCE_TRACING
        static constexpr size_t COLUMN_WIDTH = 16;

        printf("TestResource buffer:\n");
        for (size_t pos = 0; pos < BUFFER_SIZE; ++pos)
        {
            printf("|%02x%s", m_buffer[pos],
                    pos + 1 == BUFFER_SIZE || (pos + 1) % COLUMN_WIDTH == 0 ? "|\n" : "");
        }

        printf("Used %lu bytes (%lu/%lu).\n", static_cast<unsigned long>(m_nextPtr - m_buffer),
                static_cast<unsigned long>(m_numAllocations), static_cast<unsigned long>(m_numDeallocations));
#endif
    }

    virtual void* doAllocate(size_t bytes, size_t align) override
    {
        const size_t alignMod = (m_nextPtr - m_buffer) % align;
        if (alignMod != 0)
            m_nextPtr += align - alignMod;

        void* const ptr = m_nextPtr;
        m_nextPtr += bytes;

        const size_t usedBytes = m_nextPtr - m_buffer;
        if (usedBytes > BUFFER_SIZE)
        {
            printf("TestResource buffer overflow! Used %lu/%lu\n",
                    static_cast<unsigned long>(usedBytes), static_cast<unsigned long>(BUFFER_SIZE));
            exit(1);
        }

        ++m_numAllocations;

        return ptr;
    }

    virtual void doDeallocate(void* p, size_t bytes, size_t align) override
    {
        ++m_numDeallocations;
    }

    virtual bool doIsEqual(const MemoryResource& other) const noexcept override
    {
        return this == &other;
    }

    uint8_t* buffer() noexcept
    {
        return m_buffer;
    }

private:
    uint8_t m_buffer[BUFFER_SIZE];
    uint8_t* m_nextPtr = m_buffer;
    size_t m_numAllocations = 0;
    size_t m_numDeallocations = 0;
};

void printExternArray(const pmr_poc::SampleStruct& sampleStruct)
{
    // check that the extern array is valid (last data in the bit stream) -> should print "aa bb cc"
    const pmr_poc::ChildStruct& childField = sampleStruct.getChildField()[0];
    printf("%02x %02x %02x\n",
            childField.getExternArray().at(0).getBuffer()[0],
            childField.getExternArray().at(1).getBuffer()[0],
            childField.getExternArray().at(2).getBuffer()[0]);
}

//#define ENABLE_WRITER
#ifdef ENABLE_WRITER
void writeSampleStructure(zserio::BitStreamWriter& writer)
{
    // uint16Field
    writer.writeBits(0xabcd, 16);

    // stringField
    writer.writeString(std::string("This is the test string long enough to allocate!"));

    // extern field
    const std::vector<uint8_t> bitBufferData{ 0x1f };
    const zserio::BitBuffer bitBuffer(bitBufferData, 5);
    writer.writeBitBuffer(bitBuffer);

    // inplace optional field
    writer.writeBool(true);
    {
        // BigStruct
        writer.writeBits64(0xDEAD);
        writer.writeBits64(0xBEAF);
        writer.writeBits64(0xDEAD);
        writer.writeBits64(0xBEAF);
        writer.writeBits64(0xDEAD);
        writer.writeBits64(0xBEAF);
        writer.writeBits64(0xDEAD);
        writer.writeBits64(0xBEAF);
    }

    // heap optional field
    writer.writeBool(true);
    {
        // StringStruct
        writer.writeString(std::string("This is an another long string enough to allocate."));
    }

    // childField[]
    {
        // length
        writer.writeVarUInt64(1);

        // uint16Array
        std::vector<uint16_t> uint16Array{ 0x01, 0x02, 0x03, 0x04 };
        ::zserio::writeAuto(::zserio::StdIntArrayTraits<uint16_t>(), uint16Array, writer);

        // stringArray
        std::vector<std::string> stringArray{ "aaaa", "bb", "ccc", "ddd" };
        ::zserio::writeAuto(::zserio::StringArrayTraits<>(), stringArray, writer);

        // externArray
        std::vector<zserio::BitBuffer> externArray{
            zserio::BitBuffer{std::vector<uint8_t>{0xaa}},
            zserio::BitBuffer{std::vector<uint8_t>{0xbb}},
            zserio::BitBuffer{std::vector<uint8_t>{0xcc}},
        };
        ::zserio::writeAuto(::zserio::BitBufferArrayTraits<>(), externArray, writer);
    }
}
#endif

int main(int argc, char* argv[])
{
#ifdef ENABLE_WRITER
    zserio::BitStreamWriter writer;
    writeSampleStructure(writer);
    size_t bufferSize = 0;
    const uint8_t* buffer = writer.getWriteBuffer(bufferSize);
    const size_t COLUMN_WIDTH = 16;
    for (size_t i = 0; i < bufferSize; ++i)
    {
        printf("0x%02x%s", buffer[i], i + 1 == bufferSize ? "\n" :
                (i + 1) % COLUMN_WIDTH != 0 ? ", " : ",\n");
    }
#else
    const uint8_t buffer[] =
    {
        0xab, 0xcd, 0x30, 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74,
        0x65, 0x73, 0x74, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x20, 0x6c, 0x6f, 0x6e, 0x67, 0x20,
        0x65, 0x6e, 0x6f, 0x75, 0x67, 0x68, 0x20, 0x74, 0x6f, 0x20, 0x61, 0x6c, 0x6c, 0x6f, 0x63, 0x61,
        0x74, 0x65, 0x21, 0x05, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x7a, 0xb4, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x02, 0xfa, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x7a, 0xb4, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x02, 0xfa, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x7a, 0xb4, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x02, 0xfa, 0xbc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x7a, 0xb4, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x02, 0xfa, 0xbe, 0x64, 0xa8, 0xd0, 0xd2, 0xe6, 0x40, 0xd2, 0xe6, 0x40, 0xc2, 0xdc,
        0x40, 0xc2, 0xdc, 0xde, 0xe8, 0xd0, 0xca, 0xe4, 0x40, 0xd8, 0xde, 0xdc, 0xce, 0x40, 0xe6, 0xe8,
        0xe4, 0xd2, 0xdc, 0xce, 0x40, 0xca, 0xdc, 0xde, 0xea, 0xce, 0xd0, 0x40, 0xe8, 0xde, 0x40, 0xc2,
        0xd8, 0xd8, 0xde, 0xc6, 0xc2, 0xe8, 0xca, 0x5c, 0x02, 0x08, 0x00, 0x02, 0x00, 0x04, 0x00, 0x06,
        0x00, 0x08, 0x08, 0x08, 0xc2, 0xc2, 0xc2, 0xc2, 0x04, 0xc4, 0xc4, 0x06, 0xc6, 0xc6, 0xc6, 0x06,
        0xc8, 0xc8, 0xc8, 0x06, 0x11, 0x54, 0x11, 0x76, 0x11, 0x98
    };
    const size_t bufferSize = sizeof(buffer);
#endif

    {
        TestResource<> resource;
        zserio::BitStreamReader reader(buffer, bufferSize);

#if 1
        zserio::pmr::PolymorphicAllocator<pmr_poc::SampleStruct> allocator(resource);
#   if 1
        std::shared_ptr<pmr_poc::SampleStruct> sampleStruct =
                std::allocate_shared<pmr_poc::SampleStruct>(allocator, reader, resource);
        printExternArray(*sampleStruct);
#   else
        pmr_poc::SampleStruct* sampleStruct =
                new (allocator.allocate(1)) pmr_poc::SampleStruct(reader, resource);
        printExternArray(*sampleStruct);
        sampleStruct->~SampleStruct();
        allocator.deallocate(sampleStruct, 1);
#   endif
#else
        pmr_poc::SampleStruct* sampleStruct =
                new (resource.allocate(sizeof(pmr_poc::SampleStruct), alignof(pmr_poc::SampleStruct)))
                        pmr_poc::SampleStruct(reader, resource);
        printExternArray(*sampleStruct);
        sampleStruct->~SampleStruct();
        resource.deallocate(sampleStruct, sizeof(pmr_poc::SampleStruct), alignof(pmr_poc::SampleStruct));
#endif
    }

    return 0;
}

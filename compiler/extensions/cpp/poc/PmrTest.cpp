#include <limits>
#include <algorithm>
#include <memory>

#if ZSERIO_MEMORY_RESOURCE_TRACING
#   include <iomanip>
#   include <iostream>
#endif

#include "zserio/pmr/Vector.h"
#include "pmr_poc/SampleStruct.h"

template <size_t BUFFER_SIZE=256>
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

        for (size_t pos = 0; pos < BUFFER_SIZE; ++pos)
        {
            std::cout << "|" << std::hex << std::setfill('0') << std::setw(2)
                      << static_cast<uint32_t>(m_buffer[pos]) << std::dec;
            if ((pos + 1) % COLUMN_WIDTH == 0)
                std::cout << "|" << std::endl;
        }
        if (BUFFER_SIZE % COLUMN_WIDTH != 0)
            std::cout << std::endl;
#endif
    }

    virtual void* doAllocate(size_t bytes, size_t align) override
    {
        const size_t alignMod = (m_nextPtr - m_buffer) % align;
        if (alignMod != 0)
            m_nextPtr += align - alignMod;

        void* const ptr = m_nextPtr;
        m_nextPtr += bytes;
        return ptr;
    }

    virtual void doDeallocate(void* p, size_t bytes, size_t align) override
    {}

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
};

//#define ENABLE_WRITER
#ifdef ENABLE_WRITER
void writeSampleStructure(zserio::BitStreamWriter& writer)
{
    // uint8Field
    writer.writeBits(0xff, 8);

    // stringField
    writer.writeString(std::string("This is the test string long enough to allocate!"));

    // childField
    {
        // uint64Field
        writer.writeBits64(std::numeric_limits<uint64_t>::max(), 64);

        // uint16Array
        const size_t arraySize = 13;
        writer.writeVarUInt64(arraySize);
        for (size_t i = 0; i < arraySize; ++i)
            writer.writeBits(static_cast<uint16_t>(i), 16);
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
#else
    const uint8_t buffer[] =
    {
        0xff, 0x30, 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x74, 0x68, 0x65, 0x20, 0x74, 0x65,
        0x73, 0x74, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x20, 0x6c, 0x6f, 0x6e, 0x67, 0x20, 0x65,
        0x6e, 0x6f, 0x75, 0x67, 0x68, 0x20, 0x74, 0x6f, 0x20, 0x61, 0x6c, 0x6c, 0x6f, 0x63, 0x61, 0x74,
        0x65, 0x21, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0d, 0x00, 0x00, 0x00, 0x01, 0x00,
        0x02, 0x00, 0x03, 0x00, 0x04, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x08, 0x00, 0x09, 0x00,
        0x0a, 0x00, 0x0b, 0x00, 0x0c
    };
    const size_t bufferSize = sizeof(buffer);
#endif

    {
        TestResource<> resource;
        zserio::BitStreamReader reader(buffer, bufferSize);

#if 0
        zserio::pmr::PolymorphicAllocator<pmr_poc::SampleStruct> allocator(resource);
#   if 0
        std::shared_ptr<pmr_poc::SampleStruct> sampleStruct =
                std::allocate_shared<pmr_poc::SampleStruct>(allocator, reader, resource);
#   else
        pmr_poc::SampleStruct* sampleStruct =
                new (allocator.allocate(1)) pmr_poc::SampleStruct(reader, resource);
        sampleStruct->~SampleStruct();
        allocator.deallocate(sampleStruct, 1);
#   endif
#else
        pmr_poc::SampleStruct* sampleStruct =
                new (resource.allocate(sizeof(pmr_poc::SampleStruct), alignof(pmr_poc::SampleStruct)))
                        pmr_poc::SampleStruct(reader, resource);
        sampleStruct->~SampleStruct();
        resource.deallocate(sampleStruct, sizeof(pmr_poc::SampleStruct), alignof(pmr_poc::SampleStruct));
#endif
    }

    return 0;
}

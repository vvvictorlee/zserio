#include <limits>
#include <algorithm>
#include <iomanip>
#include <memory>
#include "zserio/pmr/Vector.h"

#include "pmr_poc/SampleStruct.h"

class TestResource : public zserio::pmr::MemoryResource
{
public:
    TestResource()
    {
        std::fill(m_buffer, m_buffer + BUFFER_SIZE, 0);
    }

    ~TestResource()
    {
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
    static constexpr size_t BUFFER_SIZE = 256;
    uint8_t m_buffer[BUFFER_SIZE];
    uint8_t* m_nextPtr = m_buffer;
};

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

int main(int argc, char* argv[])
{
    zserio::BitStreamWriter writer;
    writeSampleStructure(writer);
    size_t bufferSize = 0;
    const uint8_t* buffer = writer.getWriteBuffer(bufferSize);

    {
        zserio::BitStreamReader reader(buffer, bufferSize);

        using SampleStruct = pmr_poc::SampleStruct<zserio::pmr::PolymorphicAllocator<void>>;
        TestResource resource;
#if 0
        zserio::pmr::PolymorphicAllocator<SampleStruct> allocator(resource);
#   if 0
        std::shared_ptr<SampleStruct> sampleStruct =
                std::allocate_shared<SampleStruct>(allocator, reader, resource);
#   else
        SampleStruct* sampleStruct =
                new (allocator.allocate(1)) SampleStruct(reader, resource);
        sampleStruct->~SampleStruct();
        allocator.deallocate(sampleStruct, 1);
#   endif
#else
        SampleStruct* sampleStruct =
                new (resource.allocate(sizeof(SampleStruct), alignof(SampleStruct)))
                        SampleStruct(reader, resource);
        sampleStruct->~SampleStruct();
        resource.deallocate(sampleStruct, sizeof(SampleStruct), alignof(SampleStruct));
#endif
    }

    {
        zserio::BitStreamReader reader(buffer, bufferSize);

        using SampleStruct = pmr_poc::SampleStruct<>;
        SampleStruct sampleStruct(reader);
    }

    return 0;
}

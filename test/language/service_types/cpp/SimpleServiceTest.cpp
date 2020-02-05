#include <fstream>
#include <memory>

#include "gtest/gtest.h"

#include "zserio/ServiceException.h"
#include "service_types/simple_service/SimpleService.h"

namespace service_types
{
namespace simple_service
{

class SimpleServiceImpl : public SimpleService::Service
{
public:
    void powerOfTwoImpl(const Request& request, Response& response) override
    {
        int32_t value = request.getValue();
        if (value < 0)
            value = -value;
        response.setValue(static_cast<uint64_t>(value) * value);
    }
};

class SimpleServiceTest : public ::testing::Test
{
public:
    SimpleServiceTest()
    :   client(service)
    {}

protected:
    SimpleServiceImpl service;
    SimpleService::Client client;
};

TEST_F(SimpleServiceTest, serviceFullName)
{
    ASSERT_EQ(std::string("service_types.simple_service.SimpleService"),
            SimpleService::Service::serviceFullName());
}

TEST_F(SimpleServiceTest, methodName)
{
    ASSERT_EQ(std::string("powerOfTwo"), SimpleService::Service::methodNames()[0]);
}

TEST_F(SimpleServiceTest, powerOfTwo)
{
    Response response;
    Request request;

    request.setValue(13);
    client.powerOfTwoMethod(request, response);
    ASSERT_EQ(169, response.getValue());
    request.setValue(-13);
    client.powerOfTwoMethod(request, response);
    ASSERT_EQ(169, response.getValue());
    request.setValue(2);
    client.powerOfTwoMethod(request, response);
    ASSERT_EQ(4, response.getValue());
    request.setValue(-2);
    client.powerOfTwoMethod(request, response);
    ASSERT_EQ(4, response.getValue());
}

TEST_F(SimpleServiceTest, invalidServiceMethod)
{
    std::vector<uint8_t> responseData;
    ASSERT_THROW(service.callMethod("nonexistentMethod", {}, responseData), zserio::ServiceException);
}

TEST_F(SimpleServiceTest, callWithContext)
{
    void* someContext = nullptr;
    Request request{10};
    Response response;
    client.powerOfTwoMethod(request, response, someContext);
    ASSERT_EQ(100, response.getValue());
}

} // namespace simple_service
} // namespace service_types

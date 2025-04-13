#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <iomanip>
#include <string>

#include "test_TransmitReceive.h"

namespace
{
constexpr size_t bufSize{16};

class TestReceiverSocket : public ReceiveSocketThread
{
  public:
    TestReceiverSocket(const SocketInfo &info, const SocketProperties properties)
        : ReceiveSocketThread(info, properties)
    {}
    ~TestReceiverSocket() override = default;

    inline size_t get_receive_count() const { return receive_count; }

  private:
    void receiveCallback(const size_t bytes) override { receive_count++; }
    size_t receive_count{};
};

class TestTransmitterSocket : public TransmitSocketThread
{
  public:
    TestTransmitterSocket(const SocketInfo &info, const SocketProperties properties, std::chrono::milliseconds timeout)
        : TransmitSocketThread(info, properties, timeout)
    {}
    ~TestTransmitterSocket() override = default;

    inline size_t get_transmit_count() const { return transmit_count; }

  private:
    void transmitCallback(const size_t bytes) override { transmit_count++; }
    size_t transmit_count{};
};

class RunTest : public RunTest_Base<TestTransmitterSocket, TestReceiverSocket>
{
private:
    inline TestTransmitterSocket createTransmitThread(const SocketInfo &info, const SocketProperties properties) override
    {
        using namespace std::chrono_literals;
        return TestTransmitterSocket{info, properties, 1ms};
    }
    inline TestReceiverSocket createReceiveThread(const SocketInfo &info, const SocketProperties properties) override
    {
        using namespace std::chrono_literals;
        return TestReceiverSocket{info, properties};
    }
};
} // namespace

SCENARIO("Transmit and Receive", "[TCP]")
{
    using namespace std::chrono_literals;
    const SocketInfo transmitInfo{"127.0.0.1", 58585, bufSize};
    const SocketInfo receiveInfo{"127.0.0.1", 58585, bufSize};
    RunTest runTest;

    GIVEN("A pair of TCP sockets that are blocking for both the receive and transmit threads")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, false);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, false);
        auto counts = runTest(transmitInfo, transmitProperties, receiveInfo, receiveProperties);

        // Because the receive thread is stopped first, it may be off by one.
        THEN("The received message count is the same(ish) as the transmitted message count")
        {
            REQUIRE(counts.same_counts());
        }
    }
    GIVEN("A pair of TCP sockets that are nonblocking for both the receive and transmit threads")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, true);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, true);
        auto counts = runTest(transmitInfo, transmitProperties, receiveInfo, receiveProperties);

        // Non-blocking seems to not do as good.
        THEN("The received message count will be less than the transmitted message count, but not too much")
        {
            REQUIRE(counts.same_counts());
        }
    }
    GIVEN("A pair of TCP sockets that are blocking and nonblocking for the receive and transmit threads, respectively")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, true);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, false);
        auto counts = runTest(transmitInfo, transmitProperties, receiveInfo, receiveProperties);

        // Non-blocking seems to not do as good.
        THEN("The received message count may be less than the transmitted message count, but not too much")
        {
            REQUIRE(counts.same_counts());
        }
    }
    GIVEN("A pair of TCP sockets that are nonblocking and blocking for the receive and transmit threads, respectively")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, false);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, true);
        auto counts = runTest(transmitInfo, transmitProperties, receiveInfo, receiveProperties);

        // Non-blocking seems to not do as good.
        THEN("The received message count will be less than the transmitted message count, but not too much")
        {
            REQUIRE(counts.same_counts());
        }
    }
}

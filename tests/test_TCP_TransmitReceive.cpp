#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <iomanip>
#include <string>

#include "ReceiveSocketThread.h"
#include "SocketInfo.h"
#include "TransmitSocketThread.h"

namespace
{
constexpr uint16_t port{58585};
constexpr size_t bufSize{16};
inline const std::string address{"127.0.0.1"};

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

struct Counts
{
    size_t receive_count{};
    size_t transmit_count{};
};

// the purpose of this function was to avoid repeating code
Counts test_helper(const SocketInfo &transmitInfo, const SocketInfo &receiveInfo,
                   const SocketProperties transmitProperties, const SocketProperties receiveProperties,
                   const std::chrono::milliseconds txPeriod)
{
    TestTransmitterSocket tx(transmitInfo, transmitProperties, txPeriod);
    TestReceiverSocket rx(receiveInfo, receiveProperties);

    rx.start();
    tx.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    // Making the receiving thread stop first prevents the thread from hanging when it is blocking.
    rx.stop();
    tx.stop();

    // get the transmit and receive counts
    Counts rxtxCounts{};
    rxtxCounts.receive_count = rx.get_receive_count();
    rxtxCounts.transmit_count = tx.get_transmit_count();

    return rxtxCounts;
}
} // namespace

SCENARIO("Transmit and Receive", "[TCP]")
{
    using namespace std::chrono_literals;
    const SocketInfo transmitInfo{address, port, bufSize};
    const SocketInfo receiveInfo{address, port, bufSize};

    GIVEN("A pair of TCP sockets that are blocking for both the receive and transmit threads")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, false);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, false);
        auto counts = test_helper(transmitInfo, receiveInfo, transmitProperties, receiveProperties, 1ms);

        // Because the receive thread is stopped first, it may be off by one.
        THEN("The received message count is the same(ish) as the transmitted message count")
        {
            const bool approximatelyTheSame{(counts.receive_count == counts.transmit_count) ||
                                            (counts.receive_count == counts.transmit_count - 1) ||
                                            (counts.receive_count == counts.transmit_count + 1)};
            REQUIRE(approximatelyTheSame);
        }
    }
    GIVEN("A pair of TCP sockets that are nonblocking for both the receive and transmit threads")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, true);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, true);
        auto counts = test_helper(transmitInfo, receiveInfo, transmitProperties, receiveProperties, 1ms);

        // Non-blocking seems to not do as good.
        THEN("The received message count will be less than the transmitted message count, but not too much")
        {
            const bool probablyGood{(counts.receive_count > (counts.transmit_count / 2)) &&
                                    (counts.receive_count <= counts.transmit_count)};
            REQUIRE(probablyGood);
        }
    }
    GIVEN("A pair of TCP sockets that are blocking and nonblocking for the receive and transmit threads, respectively")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, true);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, false);
        auto counts = test_helper(transmitInfo, receiveInfo, transmitProperties, receiveProperties, 1ms);

        // Non-blocking seems to not do as good.
        THEN("The received message count may be less than the transmitted message count, but not too much")
        {
            const bool probablyGood{(counts.receive_count > (counts.transmit_count / 2)) &&
                                    (counts.receive_count <= counts.transmit_count)};
            REQUIRE(probablyGood);
        }
    }
    GIVEN("A pair of TCP sockets that are nonblocking and blocking for the receive and transmit threads, respectively")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, false);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, true);
        auto counts = test_helper(transmitInfo, receiveInfo, transmitProperties, receiveProperties, 1ms);

        // Non-blocking seems to not do as good.
        THEN("The received message count will be less than the transmitted message count, but not too much")
        {
            const bool probablyGood{(counts.receive_count > (counts.transmit_count / 2)) &&
                                    (counts.receive_count <= counts.transmit_count)};
            REQUIRE(probablyGood);
        }
    }
}

#ifndef TEST_TRANSMITRECEIVE_H
#define TEST_TRANSMITRECEIVE_H

#include "ReceiveSocketThread.h"
#include "TransmitSocketThread.h"

#include <iostream>
#include <type_traits>
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

struct Counts
{
    inline bool same_counts() const { return receive_count == transmit_count; }
    inline bool almost_same_counts() const { return (receive_count <= transmit_count && receive_count >= (transmit_count - 2)); }

    size_t receive_count{};
    size_t transmit_count{};
};

class RunTest
{
public:
    Counts operator()(const SocketInfo& transmitInfo, const SocketProperties transmitProperties,
                      const SocketInfo& receiveInfo, const SocketProperties receiveProperties)
    {
        using namespace std::chrono_literals;
        auto tx = TestTransmitterSocket(transmitInfo, transmitProperties, 1ms);
        auto rx = TestReceiverSocket(receiveInfo, receiveProperties);
        // rx.set_nonblocking_poll_time(1ms);

        rx.start();
        tx.start();

        std::this_thread::sleep_for(std::chrono::seconds{1});

        // Making the receiving thread stop first prevents the thread from hanging when it is blocking.
        rx.stop();
        tx.stop();

        Counts counts {};
        counts.receive_count = rx.get_receive_count();
        counts.transmit_count = tx.get_transmit_count();

        std::cout << "[TX/RX] = [" << counts.transmit_count << "/" << counts.receive_count << "]" << std::endl;
        return counts;
    }
};

#endif //TEST_TRANSMITRECEIVE_H

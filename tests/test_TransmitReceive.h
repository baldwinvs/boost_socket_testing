#ifndef TEST_TRANSMITRECEIVE_H
#define TEST_TRANSMITRECEIVE_H

#include "ReceiveSocketThread.h"
#include "TransmitSocketThread.h"

#include <type_traits>

struct Counts
{
    inline bool same_counts() const { return receive_count == transmit_count; }

    size_t receive_count{};
    size_t transmit_count{};
};

template<typename Transmit, typename Receive>
class RunTest_Base
{
public:
    Counts operator()(const SocketInfo& transmitInfo, const SocketProperties transmitProperties,
                      const SocketInfo& receiveInfo, const SocketProperties receiveProperties)
    {
        auto tx = createTransmitThread(transmitInfo, transmitProperties);
        auto rx = createReceiveThread(receiveInfo, receiveProperties);

        rx.start();
        tx.start();

        std::this_thread::sleep_for(std::chrono::seconds{1});

        // Making the receiving thread stop first prevents the thread from hanging when it is blocking.
        rx.stop();
        tx.stop();

        Counts counts {};
        counts.receive_count = rx.get_receive_count();
        counts.transmit_count = tx.get_transmit_count();
        return counts;
    }
protected:
    virtual Transmit createTransmitThread(const SocketInfo& info, const SocketProperties properties) = 0;
    virtual Receive createReceiveThread(const SocketInfo& info, const SocketProperties properties) = 0;
};

#endif //TEST_TRANSMITRECEIVE_H

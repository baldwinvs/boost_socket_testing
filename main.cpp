#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>

#include "Thread/ReceiveSocketThread.h"
#include "Thread/TransmitSocketThread.h"

constexpr uint16_t port{58585};
constexpr size_t bufSize{16};
inline const std::string address{"127.0.0.1"};

class CustomReceiverSocket : public ReceiveSocketThread
{
  public:
    CustomReceiverSocket(const SocketInfo &info, const SocketProperties properties)
        : ReceiveSocketThread(info, properties)
    {
    }
    ~CustomReceiverSocket() override = default;

  private:
    void receiveCallback(const size_t bytes) override
    {
        ReceiveSocketThread::receiveCallback(bytes);
        std::cout << __PRETTY_FUNCTION__ << " received " << std::dec << bytes << " bytes" << std::endl;
    }
};

int main()
{
    auto transmitSocketProperties = determineSocketProperties(SocketType::tcp, false, true);
    auto receiveSocketProperties = determineSocketProperties(SocketType::tcp, true, true);
    const SocketInfo transmitInfo{address, port, bufSize};
    const SocketInfo receiveInfo{address, port, bufSize};

    TransmitSocketThread tx(transmitInfo, transmitSocketProperties, std::chrono::milliseconds{1});
    CustomReceiverSocket rx(receiveInfo, receiveSocketProperties);
    rx.set_nonblocking_poll_time(std::chrono::milliseconds{1});

    rx.start();
    tx.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    // Making the receiving thread stop first prevents the thread from hanging when it is blocking.
    rx.stop();
    tx.stop();

    return 0;
}

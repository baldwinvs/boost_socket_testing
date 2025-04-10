#include <cstddef>
#include <string>
#include <iostream>
#include <iomanip>

#include "Thread/ReceiveSocketThread.h"
#include "Thread/TransmitSocketThread.h"

constexpr uint16_t port {58585};
constexpr size_t bufSize {16};
inline const std::string address {"127.0.0.1"};

class CustomReceiverSocket : public ReceiveSocketThread
{
public:
	CustomReceiverSocket(const SocketInfo& socketInfo)
		: ReceiveSocketThread(socketInfo)
	{}
	~CustomReceiverSocket() override = default;

private:
	void receiveCallback(const size_t bytes) override
	{
		ReceiveSocketThread::receiveCallback(bytes);
		std::cout << __PRETTY_FUNCTION__ << " received " << std::dec << bytes << " bytes" << std::endl;
	}
};

int main() {
    const SocketInfo transmitInfo {address, port, SocketType::udp, false, false, bufSize};
    const SocketInfo receiveInfo {address, port, SocketType::udp, true, true, bufSize};

    TransmitSocketThread tx(transmitInfo, std::chrono::milliseconds{1});
    CustomReceiverSocket rx(receiveInfo);

    rx.start();
    tx.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    // Making the receiving thread stop first prevents the thread from hanging when it is blocking.
    rx.stop();
    tx.stop();

	return 0;
}

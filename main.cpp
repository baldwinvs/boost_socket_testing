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
		std::cout << "buffer = " << "0x";
		for (size_t i = 0; i < bytes; i++) {
			auto byte = static_cast<unsigned char>(buf[i]);
			std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
		}
		std::cout << std::endl;
		std::cout << __PRETTY_FUNCTION__ << " received " << std::dec << bytes << " bytes" << std::endl;
	}
};

int main() {
    const SocketInfo transmitInfo {address, port, false, false, bufSize};
    const SocketInfo receiveInfo {address, port, true, true, bufSize};

    TransmitSocketThread tx(transmitInfo, std::chrono::milliseconds{100});
    CustomReceiverSocket rx(receiveInfo);

    rx.start();
    tx.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    tx.stop();
    rx.stop();

	return 0;
}

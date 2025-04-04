#include <cstddef>
#include <string>

#include "Thread/RxThread.h"
#include "Thread/TxThread.h"

constexpr uint16_t port {58585};
constexpr size_t bufSize {16};
inline const std::string address {"127.0.0.1"};

int main() {
    const SocketInfo transmitInfo {address, port, false, false, bufSize};
    const SocketInfo receiveInfo {address, port, true, true, bufSize};

    TxThread tx(transmitInfo, std::chrono::milliseconds{100});
    RxThread rx(receiveInfo);

    rx.start();
    tx.start();

    std::this_thread::sleep_for(std::chrono::seconds{1});

    tx.stop();
    rx.stop();

	return 0;
}

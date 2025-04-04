#include "TransmitSocketThread.h"

#include "Wrapper_AsioUdp.h"

void TransmitSocketThread::run() {
    auto udp = Wrapper_AsioUdp(socketInfo);

    const double data {1.5678};
    std::memcpy(&buf[0], &data, sizeof(data));

    while (running) {
        auto bytes = udp.send(buf.get());
        std::this_thread::sleep_for(sleep_time);
    }
}

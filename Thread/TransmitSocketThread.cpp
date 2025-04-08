#include "TransmitSocketThread.h"

#include "../Socket/Wrapper_AsioUdp.h"

void TransmitSocketThread::run()
{
    createSocket();

    const double data {1.5678};
    std::memcpy(&buf[0], &data, sizeof(data));

    while (running) {
        auto bytes = socket->send(buf.get());
        std::this_thread::sleep_for(sleep_time);
    }
}

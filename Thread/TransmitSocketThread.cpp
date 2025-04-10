#include "TransmitSocketThread.h"

#include "../Socket/Wrapper_AsioUdp.h"

void TransmitSocketThread::run()
{
    createSocket();

    const double data {1.5678};
    std::memcpy(&buf[0], &data, sizeof(data));

    while (running) {
        auto bytes = socket->send(buf.get(), socketInfo.bufferSize);
        switch (socketInfo.properties)
        {
        case SocketProperties::tcp_send_nonblocking:
            while (bytes < socketInfo.bufferSize)
            {
                std::this_thread::yield();
                bytes += socket->send(buf.get(), socketInfo.bufferSize - bytes);
            }
            break;
        case SocketProperties::udp_send_nonblocking:
            // Yield control of the hardware thread and try to send again.
            while (bytes != socketInfo.bufferSize)
            {
                std::this_thread::yield();
                bytes = socket->send(buf.get(), socketInfo.bufferSize);
            }
        default:
            // do nothing
            break;
        }
        std::this_thread::sleep_for(sleep_time);
    }
}

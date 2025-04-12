#include "TransmitSocketThread.h"

#include "../Socket/Wrapper_AsioUdp.h"

void TransmitSocketThread::run()
{
    createSocket();

    const double data {1.5678};
    std::memcpy(&buf[0], &data, sizeof(data));

    while (running) {
        auto bytes = socket->send(buf.get(), info.bufferSize);
        switch (properties)
        {
        case SocketProperties::tcp_send_nonblocking:
            while (bytes < info.bufferSize)
            {
                std::this_thread::yield();
                bytes += socket->send(buf.get(), info.bufferSize - bytes);
            }
            break;
        case SocketProperties::udp_send_nonblocking:
            // Yield control of the hardware thread and try to send again.
            while (bytes != info.bufferSize)
            {
                std::this_thread::yield();
                bytes = socket->send(buf.get(), info.bufferSize);
            }
        default:
            // do nothing
            break;
        }
        std::this_thread::sleep_for(sleep_time);
    }
}

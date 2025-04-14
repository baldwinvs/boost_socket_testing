#include "ReceiveSocketThread.h"
#include "../Socket/Wrapper_AsioUdp.h"
#include <iostream>
#include <iomanip>

ReceiveSocketThread::~ReceiveSocketThread()
{

}

void ReceiveSocketThread::receiveCallback(const size_t bytes)
{
    std::cout << "buffer = " << "0x";
    for (size_t i = 0; i < bytes; i++) {
        auto byte = static_cast<unsigned char>(buf[i]);
        std::cout << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte);
    }
    std::cout << std::endl;
}

void ReceiveSocketThread::run()
{
    using namespace std::chrono_literals;
    createSocket();
    while (running) {
        //because the socket is created within this function, cannot stop the socket, or it's io_service
        //  doing it this way REQUIRE that the receiving socket is non-blocking.
        auto bytes = socket->recv(buf.get(), info.bufferSize);
        switch (properties)
        {
        case SocketProperties::tcp_receive_nonblocking:
            while (bytes < info.bufferSize)
            {
                yield();
                bytes += socket->recv(buf.get(), info.bufferSize - bytes);
            }
            //need a sleep if non-blocking
            std::this_thread::sleep_for(1ms);
            break;
        case SocketProperties::udp_receive_nonblocking:
            //need a sleep if non-blocking
            std::this_thread::sleep_for(1ms);
            break;
        default:
            // do nothing
            break;
        }

        if (bytes > 0) {
            receiveCallback(bytes);
        }
    }
}

void ReceiveSocketThread::set_nonblocking_poll_time(const std::chrono::milliseconds& poll_time_ms)
{
    switch (properties)
    {
    case SocketProperties::tcp_receive_nonblocking:
    case SocketProperties::udp_receive_nonblocking:
        this->poll_time_ms = poll_time_ms;
        break;
    default:
        // do nothing
        break;
    }
}

void ReceiveSocketThread::yield() const
{
    if (poll_time_ms == std::chrono::milliseconds(0))
        std::this_thread::yield();
    else if (poll_time_ms > std::chrono::milliseconds(0))
        std::this_thread::sleep_for(poll_time_ms);
}
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
    createSocket();
    while (running) {
        //because the socket is created within this function, cannot stop the socket, or it's io_service
        //  doing it this way REQUIRE that the receiving socket is non-blocking.
        auto bytes = socket->recv(buf.get(), info.bufferSize);
        switch (properties)
        {
        case SocketProperties::tcp_receive_nonblocking:
            //need a sleep if non-blocking
            std::this_thread::sleep_for(std::chrono::microseconds(100));
            while (bytes < info.bufferSize)
            {
                bytes += socket->recv(buf.get(), info.bufferSize - bytes);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            //need a sleep if non-blocking
            break;
        case SocketProperties::udp_receive_nonblocking:
            //need a sleep if non-blocking
            std::this_thread::sleep_for(std::chrono::microseconds(100));
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
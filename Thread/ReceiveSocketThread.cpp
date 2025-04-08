#include "ReceiveSocketThread.h"
#include "../Socket/Wrapper_AsioUdp.h"
#include <iostream>
#include <iomanip>

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
        auto bytes = socket->recv(buf.get());
        if (bytes > 0) {
            receiveCallback(bytes);
        }
        //need a sleep if non-blocking
        if (socketInfo.non_blocking) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }
}
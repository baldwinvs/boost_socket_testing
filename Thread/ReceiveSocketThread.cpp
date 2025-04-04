#include "ReceiveSocketThread.h"
#include "Wrapper_AsioUdp.h"

void ReceiveSocketThread::run()
{
    Wrapper_AsioUdp udp{socketInfo};
    while (running) {
        //because the socket is created within this function, cannot stop the socket, or it's io_service
        //  doing it this way REQUIRE that the receiving socket is non-blocking.
        auto bytes = udp.recv(buf.get());
        if (bytes > 0) {
            receiveCallback(bytes);
        }
        //need a sleep if non-blocking
        if (socketInfo.non_blocking) {
            std::this_thread::sleep_for(std::chrono::microseconds(100));
        }
    }
}
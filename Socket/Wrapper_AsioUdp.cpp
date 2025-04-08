#include "Wrapper_AsioUdp.h"
#include <iostream>

Wrapper_AsioUdp::Wrapper_AsioUdp(const SocketInfo& socketInfo)
    : Base_AsioSocket(socketInfo)
    , socket {ioContext}
    , endpoint {boost::asio::ip::address::from_string(socketInfo.ip), socketInfo.port}
{
    socket.open(boost::asio::ip::udp::v4());
    socket.non_blocking(socketInfo.non_blocking);
    if (socketInfo.receiver) {
        socket.bind(endpoint);
    }
}

Wrapper_AsioUdp::~Wrapper_AsioUdp()
{
    socket.close();
}

size_t Wrapper_AsioUdp::send(const unsigned char* const buf)
{
    size_t bytesSent {};
    if (!socketInfo.receiver) {
        bytesSent = socket.send_to(boost::asio::buffer(buf, socketInfo.bufferSize), endpoint);
    }
    return bytesSent;
}

size_t Wrapper_AsioUdp::recv(unsigned char* buf)
{
    size_t bytesReceived {};
    if (socketInfo.receiver) {
        if (socketInfo.non_blocking) {
            boost::system::error_code error;
            bytesReceived = socket.receive_from(boost::asio::buffer(buf, socketInfo.bufferSize), endpoint, 0, error);

            if (error == boost::asio::error::would_block) {
                //do nothing
            } else if (error) {
                std::cerr << "Receive error: " << error.message() << std::endl;
            } else {
                std::cerr << "Received " << bytesReceived << std::endl;
            }
        }
        else {
            bytesReceived = socket.receive_from(boost::asio::buffer(buf, socketInfo.bufferSize), endpoint);
        }
    }
    return bytesReceived;
}

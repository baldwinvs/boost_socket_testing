#include "Wrapper_AsioTcp.h"
#include <string>
#include <iostream>

#include <boost/asio/connect.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>

Wrapper_AsioTcp::Wrapper_AsioTcp(const SocketInfo &socketInfo)
    : Base_AsioSocket(socketInfo)
    , socket {}
{
    connect();
}

Wrapper_AsioTcp::~Wrapper_AsioTcp()
{
    socket->close();
}

void Wrapper_AsioTcp::connect()
{
    if (socketInfo.receiver) {
        boost::asio::ip::tcp::resolver resolver{ioContext};
        const auto endpoint = resolver.resolve(socketInfo.ip, std::to_string(socketInfo.port));
        socket = std::make_unique<boost::asio::ip::tcp::socket>(ioContext);
        boost::asio::connect(*(socket.get()), endpoint);
        socket->non_blocking(socketInfo.non_blocking); //non-blocking after connection makes it work, HUZZAHHHH
    }
    else {
        const auto ip = boost::asio::ip::address::from_string(socketInfo.ip);
        const auto endpoint = boost::asio::ip::tcp::endpoint(ip, socketInfo.port);
        boost::asio::ip::tcp::acceptor acceptor{ioContext, endpoint};
        socket = std::make_unique<boost::asio::ip::tcp::socket>(ioContext);
        acceptor.accept(*(socket.get()));
    }
}

size_t Wrapper_AsioTcp::send(const unsigned char* const buf)
{
    size_t bytesSent {};
    if (!socketInfo.receiver) {
        bytesSent = boost::asio::write(*socket, boost::asio::buffer(buf, socketInfo.bufferSize));
    }
    return bytesSent;
}

size_t Wrapper_AsioTcp::recv(unsigned char* buf)
{
    size_t bytesReceived {};
    if (socketInfo.receiver) {
        if (socketInfo.non_blocking) {
            boost::system::error_code ec {};
            bytesReceived = boost::asio::read(*(socket.get()), boost::asio::buffer(buf, socketInfo.bufferSize), ec);

            //maybe need error handling code
            if (ec == boost::asio::error::would_block || ec == boost::asio::error::try_again) {
                // No data available right now — this is expected in non-blocking mode
                // You can retry later (e.g., using a loop, timer, or select/epoll if integrating with other code)
            } else if (ec) {
                // Some other error occurred — handle it
                std::cerr << "Socket read error: " << ec.message() << "\n";
            } else {
                // Success! Process the `bytes_read` of data in `buffer`
            }
        }
        else {
            bytesReceived = boost::asio::read(*(socket.get()), boost::asio::buffer(buf, socketInfo.bufferSize));
        }
    }
    return bytesReceived;
}

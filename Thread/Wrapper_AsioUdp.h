#ifndef WRAPPER_ASIOUDP_H
#define WRAPPER_ASIOUDP_H

#include "SocketInfo.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

class Wrapper_AsioUdp {
public:
    explicit Wrapper_AsioUdp(const SocketInfo& socketInfo);
    ~Wrapper_AsioUdp();
    boost::asio::io_context& io_context() { return ioContext; }

    size_t send(const unsigned char* const buf);
    size_t recv(unsigned char* buf);
private:
    boost::asio::io_context ioContext;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint endpoint;
    bool receiver;
    bool nonBlocking;
    uint32_t bufferSize;
};

#endif //WRAPPER_ASIOUDP_H

#ifndef BASE_ASIOSOCKET_H
#define BASE_ASIOSOCKET_H

#include "SocketInfo.h"

#include <boost/asio/io_context.hpp>

class Base_AsioSocket {
public:
    explicit Base_AsioSocket(const SocketInfo& socketInfo);

    boost::asio::io_context& get_io_context() { return ioContext; }
    virtual size_t send(const unsigned char* const buf) = 0;
    virtual size_t recv(unsigned char* buf) = 0;

protected:
    boost::asio::io_context ioContext;
    SocketInfo socketInfo;
};

#endif //BASE_ASIOSOCKET_H

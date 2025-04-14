#ifndef BASE_ASIOSOCKET_H
#define BASE_ASIOSOCKET_H

#include "SocketInfo.h"

#include <boost/asio/io_context.hpp>

class Base_AsioSocket {
public:
    explicit Base_AsioSocket(const SocketInfo& info, const SocketProperties properties);

    boost::asio::io_context& get_io_context() { return ioContext; }
    SocketInfo get_socket_info() const { return info; }
    SocketProperties get_socket_properties() const { return properties; }
    virtual size_t send(const unsigned char* const buf, const size_t size) = 0;
    virtual size_t recv(unsigned char* buf, const size_t size) = 0;

protected:
    boost::asio::io_context ioContext;
    SocketInfo info;
    SocketProperties properties;
};

#endif //BASE_ASIOSOCKET_H

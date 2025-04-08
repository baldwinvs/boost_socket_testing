#ifndef WRAPPER_ASIOUDP_H
#define WRAPPER_ASIOUDP_H

#include "Base_AsioSocket.h"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

class Wrapper_AsioUdp : public Base_AsioSocket
{
public:
    explicit Wrapper_AsioUdp(const SocketInfo& socketInfo);
    ~Wrapper_AsioUdp();

    boost::asio::ip::udp::socket* get_socket() { return &socket; }

    size_t send(const unsigned char* const buf) override;
    size_t recv(unsigned char* buf) override;
private:
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint endpoint;
};

#endif //WRAPPER_ASIOUDP_H

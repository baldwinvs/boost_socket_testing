#ifndef WRAPPER_ASIOTCP_H
#define WRAPPER_ASIOTCP_H

#include "Base_AsioSocket.h"
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

class Wrapper_AsioTcp : public Base_AsioSocket
{
public:
    explicit Wrapper_AsioTcp(const SocketInfo& socketInfo);
    ~Wrapper_AsioTcp();

    void connect();
    boost::asio::ip::tcp::socket* get_socket() { return socket.get(); }

    size_t send(const unsigned char* const buf) override;
    size_t recv(unsigned char* buf) override;

private:
    // The socket is a pointer because it can't be properly initialized until
    // the body of the constructor (or later?)
    // NOTE: Could make a connect function that will perform the operation later.
    std::unique_ptr<boost::asio::ip::tcp::socket> socket;
};

#endif //WRAPPER_ASIOTCP_H

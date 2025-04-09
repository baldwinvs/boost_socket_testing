#ifndef WRAPPER_ASIOTCP_H
#define WRAPPER_ASIOTCP_H

#include "Base_AsioSocket.h"
#include "../Utility/ObserverPtr.h"
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

class Wrapper_AsioTcp : public Base_AsioSocket
{
public:
    explicit Wrapper_AsioTcp(const SocketInfo& socketInfo);
    ~Wrapper_AsioTcp();

    void connect();
    FixedObserverPtr<boost::asio::ip::tcp::socket> get_socket();

    size_t send(const unsigned char* const buf) override;
    size_t recv(unsigned char* buf) override;
    size_t send(ImmutableObserverPtr<unsigned char> buf);
    size_t recv(MutableObserverPtr<unsigned char> buf);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

#endif //WRAPPER_ASIOTCP_H

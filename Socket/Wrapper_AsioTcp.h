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
    explicit Wrapper_AsioTcp(const SocketInfo& info, const SocketProperties properties);
    ~Wrapper_AsioTcp();

    void connect();
    FixedObserverPtr<boost::asio::ip::tcp::socket> get_socket();

    size_t recv(unsigned char* buf, size_t size) override;
    size_t recv(MutableObserverPtr<unsigned char> buf, size_t size) const;
    size_t send(const unsigned char* const buf, size_t size) override;
    size_t send(ImmutableObserverPtr<unsigned char> buf, size_t size);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

#endif //WRAPPER_ASIOTCP_H

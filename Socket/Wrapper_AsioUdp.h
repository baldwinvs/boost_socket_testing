#ifndef WRAPPER_ASIOUDP_H
#define WRAPPER_ASIOUDP_H

#include "Base_AsioSocket.h"
#include "../Utility/ObserverPtr.h"
#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/udp.hpp>

class Wrapper_AsioUdp : public Base_AsioSocket
{
public:
    explicit Wrapper_AsioUdp(const SocketInfo& info, const SocketProperties properties);
    ~Wrapper_AsioUdp();

    size_t recv(unsigned char* buf, size_t size) override;
    size_t recv(MutableObserverPtr<unsigned char> buf, size_t size) const;
    size_t send(const unsigned char* const buf, size_t size) override;
    size_t send(ImmutableObserverPtr<unsigned char> buf, size_t size);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

#endif //WRAPPER_ASIOUDP_H

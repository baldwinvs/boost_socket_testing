#include "Wrapper_AsioUdp.h"
#include <iostream>

namespace
{
inline size_t capSize(const size_t size, const size_t max)
{
    return (size > max) ? max : size;
}
}

struct Wrapper_AsioUdp::Impl
{
    explicit Impl(Wrapper_AsioUdp *const udp)
        : udpObserver{udp}, socket{udpObserver->ioContext},
          endpoint{boost::asio::ip::address::from_string(udpObserver->socketInfo.ip), udpObserver->socketInfo.port}
    {
        socket.open(boost::asio::ip::udp::v4());
        switch (udpObserver->socketInfo.properties)
        {
        case SocketProperties::udp_receive_blocking:
        case SocketProperties::udp_receive_nonblocking:
            socket.non_blocking(SocketProperties::udp_receive_nonblocking == udpObserver->socketInfo.properties);
            socket.bind(endpoint);
            break;
        case SocketProperties::udp_send_nonblocking:
            socket.non_blocking(true);
            break;
        default:
            //do nothing
            break;
        }
    }

    ~Impl()
    {
        socket.close();
    }
    FixedObserverPtr<Wrapper_AsioUdp> udpObserver;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint endpoint;

    size_t receive_blocking(MutableObserverPtr<unsigned char> buf, const size_t size);
    size_t receive_nonblocking(MutableObserverPtr<unsigned char> buf, const size_t size);
    size_t send_blocking(ImmutableObserverPtr<unsigned char> buf, const size_t size);
    size_t send_nonblocking(ImmutableObserverPtr<unsigned char> buf, const size_t size);
};

Wrapper_AsioUdp::Wrapper_AsioUdp(const SocketInfo& socketInfo)
    : Base_AsioSocket(socketInfo), impl{std::make_unique<Impl>(this)}
{}

Wrapper_AsioUdp::~Wrapper_AsioUdp() = default;

FixedObserverPtr<boost::asio::ip::udp::socket> Wrapper_AsioUdp::get_socket()
{
    return FixedObserverPtr<boost::asio::ip::udp::udp::socket>{&impl->socket};
}


size_t Wrapper_AsioUdp::recv(unsigned char* buf, size_t size)
{
    return recv(MutableObserverPtr<unsigned char>{buf}, size);
}
size_t Wrapper_AsioUdp::recv(MutableObserverPtr<unsigned char> buf, size_t size) const
{
    size_t bytesReceived {};
    switch (socketInfo.properties)
    {
    case SocketProperties::udp_receive_blocking:
        bytesReceived = impl->receive_blocking(buf, capSize(size, socketInfo.bufferSize));
        break;
    case SocketProperties::udp_receive_nonblocking:
        bytesReceived = impl->receive_nonblocking(buf, capSize(size, socketInfo.bufferSize));
        break;
    default:
        // do nothing, shouldn't occur
        break;
    }
    return bytesReceived;
}

size_t Wrapper_AsioUdp::send(const unsigned char* const buf, size_t size)
{
    return send(ImmutableObserverPtr<unsigned char>{buf}, size);
}

size_t Wrapper_AsioUdp::send(ImmutableObserverPtr<unsigned char> buf, size_t size)
{
    size_t bytesSent {};
    switch (socketInfo.properties)
    {
    case SocketProperties::udp_send_blocking:
        bytesSent = impl->send_blocking(buf, capSize(size, socketInfo.bufferSize));
        break;
    case SocketProperties::udp_send_nonblocking:
        bytesSent = impl->send_nonblocking(buf, capSize(size, socketInfo.bufferSize));
        break;
    default:
        // do nothing, shouldn't occur
        break;
    }
    return bytesSent;
}

size_t Wrapper_AsioUdp::Impl::receive_blocking(MutableObserverPtr<unsigned char> buf, const size_t size)
{
    return socket.receive_from(boost::asio::buffer(buf.get(), size), endpoint);
}

size_t Wrapper_AsioUdp::Impl::receive_nonblocking(MutableObserverPtr<unsigned char> buf, const size_t size)
{
    boost::system::error_code ec{};
    auto bytesReceived = socket.receive_from(boost::asio::buffer(buf.get(), size), endpoint, 0, ec);

    if (ec == boost::asio::error::would_block)
    {
        // No data available right now — this is expected in non-blocking mode
    }
    else if (ec)
    {
        // Handle other errors
        std::cerr << "UDP Receive (nonblocking) error: " << ec.message() << std::endl;
    }
    else
    {
        // Success! Process the `bytes_read` of data in `buffer`
    }
    return bytesReceived;
}

size_t Wrapper_AsioUdp::Impl::send_blocking(ImmutableObserverPtr<unsigned char> buf, const size_t size)
{
    return socket.send_to(boost::asio::buffer(buf.get(), size), endpoint);
}

size_t Wrapper_AsioUdp::Impl::send_nonblocking(ImmutableObserverPtr<unsigned char> buf, const size_t size)
{
    boost::system::error_code ec{};
    auto bytesSent = socket.send_to(boost::asio::buffer(buf.get(), size), endpoint);

    if (ec == boost::asio::error::would_block)
    {
        // No data available right now — this is expected in non-blocking mode
    }
    else if (ec)
    {
        // Handle other errors
        std::cerr << "UDP Send (nonblocking) error: " << ec.message() << std::endl;
    }
    else
    {
        // Success! Process the `bytes_read` of data in `buffer`
    }
    return bytesSent;
}
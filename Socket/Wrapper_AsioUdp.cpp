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
          endpoint{boost::asio::ip::address::from_string(udpObserver->info.ip), udpObserver->info.port}
    {
        socket.open(boost::asio::ip::udp::v4());
        switch (udpObserver->properties)
        {
        case SocketProperties::udp_receive_blocking:
        case SocketProperties::udp_receive_nonblocking:
            socket.non_blocking(SocketProperties::udp_receive_nonblocking == udpObserver->properties);
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
        if (socket.is_open())
        {
            // no shutdown for UDP
            socket.close();
        }
    }
    FixedObserverPtr<Wrapper_AsioUdp> udpObserver;
    boost::asio::ip::udp::socket socket;
    boost::asio::ip::udp::endpoint endpoint;

    size_t receive_blocking(MutableObserverPtr<unsigned char> buf, const size_t size);
    size_t receive_nonblocking(MutableObserverPtr<unsigned char> buf, const size_t size);
    size_t send_blocking(ImmutableObserverPtr<unsigned char> buf, const size_t size);
    size_t send_nonblocking(ImmutableObserverPtr<unsigned char> buf, const size_t size);
};

Wrapper_AsioUdp::Wrapper_AsioUdp(const SocketInfo& info, const SocketProperties properties)
    : Base_AsioSocket(info, properties), impl{std::make_unique<Impl>(this)}
{}

Wrapper_AsioUdp::~Wrapper_AsioUdp() = default;

size_t Wrapper_AsioUdp::recv(unsigned char* buf, size_t size)
{
    return recv(MutableObserverPtr<unsigned char>{buf}, size);
}
size_t Wrapper_AsioUdp::recv(MutableObserverPtr<unsigned char> buf, size_t size) const
{
    size_t bytesReceived {};
    switch (properties)
    {
    case SocketProperties::udp_receive_blocking:
        bytesReceived = impl->receive_blocking(buf, capSize(size, info.bufferSize));
        break;
    case SocketProperties::udp_receive_nonblocking:
        bytesReceived = impl->receive_nonblocking(buf, capSize(size, info.bufferSize));
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
    switch (properties)
    {
    case SocketProperties::udp_send_blocking:
        bytesSent = impl->send_blocking(buf, capSize(size, info.bufferSize));
        break;
    case SocketProperties::udp_send_nonblocking:
        bytesSent = impl->send_nonblocking(buf, capSize(size, info.bufferSize));
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
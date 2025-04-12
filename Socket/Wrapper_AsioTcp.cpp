#include "Wrapper_AsioTcp.h"

#include <boost/asio/connect.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <iostream>
#include <string>

namespace
{
inline size_t capSize(const size_t size, const size_t max)
{
    return (size > max) ? max : size;
}
}

struct Wrapper_AsioTcp::Impl
{
    explicit Impl(Wrapper_AsioTcp *const tcp) : tcpObserver{tcp}
    {}

    ~Impl()
    {
        socket->close();
    }
    // The socket is a pointer because it can't be properly initialized until
    // the body of the constructor (or later?)
    // NOTE: Could make a connect function that will perform the operation later.
    std::unique_ptr<boost::asio::ip::tcp::socket> socket;

    FixedObserverPtr<Wrapper_AsioTcp> tcpObserver;
    void connect_receive();
    void connect_send();
    size_t receive_blocking(MutableObserverPtr<unsigned char> buf, const size_t size);
    size_t receive_nonblocking(MutableObserverPtr<unsigned char> buf, const size_t size);
    size_t send_blocking(ImmutableObserverPtr<unsigned char> buf, const size_t size);
    size_t send_nonblocking(ImmutableObserverPtr<unsigned char> buf, const size_t size);
};

Wrapper_AsioTcp::Wrapper_AsioTcp(const SocketInfo &info, const SocketProperties properties)
    : Base_AsioSocket(info, properties), impl{std::make_unique<Impl>(this)}
{
    connect();
}

Wrapper_AsioTcp::~Wrapper_AsioTcp() = default;

void Wrapper_AsioTcp::connect()
{
    switch (properties)
    {
    case SocketProperties::tcp_receive_blocking:
    case SocketProperties::tcp_receive_nonblocking: {
        impl->connect_receive();
    }
    break;
    case SocketProperties::tcp_send_blocking:
    case SocketProperties::tcp_send_nonblocking: {
        impl->connect_send();
    }
    break;
    default:
        // do nothing, shouldn't occur
        break;
    }
}

FixedObserverPtr<boost::asio::ip::tcp::socket> Wrapper_AsioTcp::get_socket()
{
    return FixedObserverPtr<boost::asio::ip::tcp::socket>{impl->socket.get()};
}

size_t Wrapper_AsioTcp::recv(unsigned char* buf, size_t size)
{
    return recv(MutableObserverPtr<unsigned char>{buf}, size);
}

size_t Wrapper_AsioTcp::recv(MutableObserverPtr<unsigned char> buf, size_t size) const
{
    size_t bytesReceived{};
    switch (properties)
    {
    case SocketProperties::tcp_receive_blocking:
        bytesReceived = impl->receive_blocking(buf, capSize(size, info.bufferSize));
        break;
    case SocketProperties::tcp_receive_nonblocking:
        bytesReceived = impl->receive_nonblocking(buf, capSize(size, info.bufferSize));
        break;
    default:
        // do nothing, shouldn't occur
        break;
    }
    return bytesReceived;
}

size_t Wrapper_AsioTcp::send(const unsigned char *const buf, size_t size)
{
    return send(ImmutableObserverPtr<unsigned char>{buf}, size);
}

size_t Wrapper_AsioTcp::send(ImmutableObserverPtr<unsigned char> buf, size_t size)
{
    size_t bytesSent{};
    switch (properties)
    {
    case SocketProperties::tcp_send_blocking:
        bytesSent = impl->send_blocking(buf, capSize(size, info.bufferSize));
        break;
    case SocketProperties::tcp_send_nonblocking:
        bytesSent = impl->send_nonblocking(buf, capSize(size, info.bufferSize));
        break;
    default:
        // do nothing, shouldn't occur
        break;
    }
    return bytesSent;
}

void Wrapper_AsioTcp::Impl::connect_receive()
{
    boost::asio::ip::tcp::resolver resolver{tcpObserver->ioContext};
    const auto endpoint = resolver.resolve(tcpObserver->info.ip, std::to_string(tcpObserver->info.port));
    socket = std::make_unique<boost::asio::ip::tcp::socket>(tcpObserver->ioContext);
    boost::asio::connect(*(socket.get()), endpoint);
    // non-blocking AFTER connection makes it work, HUZZAHHHH!
    socket->non_blocking(SocketProperties::tcp_receive_nonblocking == tcpObserver->properties);
}

void Wrapper_AsioTcp::Impl::connect_send()
{
    const auto ip = boost::asio::ip::address::from_string(tcpObserver->info.ip);
    const auto endpoint = boost::asio::ip::tcp::endpoint(ip, tcpObserver->info.port);
    boost::asio::ip::tcp::acceptor acceptor{tcpObserver->ioContext, endpoint};
    socket = std::make_unique<boost::asio::ip::tcp::socket>(tcpObserver->ioContext);
    acceptor.accept(*(socket.get()));
    socket->non_blocking(SocketProperties::tcp_send_nonblocking == tcpObserver->properties);
}

size_t Wrapper_AsioTcp::Impl::receive_blocking(MutableObserverPtr<unsigned char> buf, const size_t size)
{
        return boost::asio::read(*(socket.get()), boost::asio::buffer(buf.get(), size));
}

size_t Wrapper_AsioTcp::Impl::receive_nonblocking(MutableObserverPtr<unsigned char> buf, const size_t size)
{
    boost::system::error_code ec{};
    auto bytesReceived = boost::asio::read(*(socket.get()), boost::asio::buffer(buf.get(), size), ec);

    // maybe need error handling code
    if (ec == boost::asio::error::would_block || ec == boost::asio::error::try_again)
    {
        // No data available right now — this is expected in non-blocking
        // mode You can retry later (e.g., using a loop, timer, or
        // select/epoll if integrating with other code)
    }
    else if (ec)
    {
        // Some other error occurred — handle it
        std::cerr << "TCP Receive (nonblocking) error: " << ec.message() << std::endl;
    }
    else
    {
        // Success! Process the `bytes_read` of data in `buffer`
    }
    return bytesReceived;
}

size_t Wrapper_AsioTcp::Impl::send_blocking(ImmutableObserverPtr<unsigned char> buf, const size_t size)
{
    return boost::asio::write(*socket, boost::asio::buffer(buf.get(), size));
}

size_t Wrapper_AsioTcp::Impl::send_nonblocking(ImmutableObserverPtr<unsigned char> buf, const size_t size)
{
    boost::system::error_code ec {};
    auto bytesSent = boost::asio::write(*(socket.get()), boost::asio::buffer(buf.get(), size), ec);

    if (ec == boost::asio::error::would_block)
    {
        // Partial write - track bytesSent, retry later
    }
    else if (ec)
    {
        // Handle other errors
        std::cerr << "TCP Send (nonblocking) error: " << ec.message() << std::endl;
    }
    else
    {
        // All data sent successfully
    }
    return bytesSent;
}
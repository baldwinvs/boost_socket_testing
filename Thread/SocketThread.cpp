#include "SocketThread.h"

#include "../Socket/Wrapper_AsioTcp.h"
#include "../Socket/Wrapper_AsioUdp.h"

SocketThread::SocketThread(const SocketInfo& info, const SocketProperties properties)
    : info {info}
    , properties {properties}
    , buf {std::make_unique<unsigned char[]>(info.bufferSize)}
    , socket {}
{}

SocketThread::~SocketThread() = default;

void SocketThread::createSocket()
{
    switch (properties) {
    case SocketProperties::udp_receive_blocking:
    case SocketProperties::udp_receive_nonblocking:
    case SocketProperties::udp_send_blocking:
    case SocketProperties::udp_send_nonblocking:
        socket = std::make_unique<Wrapper_AsioUdp>(info, properties);
        break;
    case SocketProperties::tcp_receive_blocking:
    case SocketProperties::tcp_receive_nonblocking:
    case SocketProperties::tcp_send_blocking:
    case SocketProperties::tcp_send_nonblocking:
        socket = std::make_unique<Wrapper_AsioTcp>(info, properties);
        break;
    }
}

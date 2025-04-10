#include "SocketThread.h"

#include "../Socket/Wrapper_AsioTcp.h"
#include "../Socket/Wrapper_AsioUdp.h"

SocketThread::SocketThread(const SocketInfo& socketInfo)
    : socketInfo {socketInfo}
    , buf {std::make_unique<unsigned char[]>(socketInfo.bufferSize)}
    , socket {}
{}

SocketThread::~SocketThread() = default;

void SocketThread::createSocket()
{
    switch (socketInfo.properties) {
    case SocketProperties::udp_receive_blocking:
    case SocketProperties::udp_receive_nonblocking:
    case SocketProperties::udp_send_blocking:
    case SocketProperties::udp_send_nonblocking:
        socket = std::make_unique<Wrapper_AsioUdp>(socketInfo);
        break;
    case SocketProperties::tcp_receive_blocking:
    case SocketProperties::tcp_receive_nonblocking:
    case SocketProperties::tcp_send_blocking:
    case SocketProperties::tcp_send_nonblocking:
        socket = std::make_unique<Wrapper_AsioTcp>(socketInfo);
        break;
    }
}

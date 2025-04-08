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
    switch (socketInfo.type) {
        case SocketType::udp:
            socket = std::make_unique<Wrapper_AsioUdp>(socketInfo);
        break;
        case SocketType::tcp:
            socket = std::make_unique<Wrapper_AsioTcp>(socketInfo);
        break;
    }
}

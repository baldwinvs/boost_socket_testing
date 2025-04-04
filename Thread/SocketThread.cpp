#include "SocketThread.h"

SocketThread::SocketThread(const SocketInfo& socketInfo)
    : socketInfo {socketInfo}
    , buf {std::make_unique<unsigned char[]>(socketInfo.bufferSize)}
{}

SocketThread::~SocketThread() = default;
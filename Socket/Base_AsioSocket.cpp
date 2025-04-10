#include "Base_AsioSocket.h"

Base_AsioSocket::Base_AsioSocket(const SocketInfo &info, const SocketProperties properties)
    : ioContext {}
    , info {info}
    , properties {properties}
{}
